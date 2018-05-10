from project import app
from flask import render_template, request, session, send_from_directory
from flask_wtf import FlaskForm
import os
import json
import glob
import subprocess

@app.route('/case', methods = ['GET', 'POST'])
def case():
	print "Case!!!"
	resp = {}
	if 'username' not in session:
		resp['result_type'] = "info"
		resp['result'] = "Please log in!"
		return render_template('accounts/login.html', resp = resp)
	if request.method == 'GET':
		case_name = request.args.get('case_name')
		if case_name is None:
			resp['result_type'] = "danger"
			resp['result'] = "Please specify the case!"
			resp['cases'] = session['cases']
			return render_template('accounts/index.html', resp = resp)
		try:
			if case_name not in session['cases']:
				print "Case name not in session"
				resp['result_type'] = "danger"
				resp['result'] = "Specified case name is wrong!"
				resp['cases'] = session['cases']
				return render_template('accounts/index.html', resp = resp)
			
			# added here
			try:
				case_data = json.load(open('../data/json/' + case_name))
			except Exception as e:
				print "Could not open file"
				return render_template('accounts/index.html', resp = resp)
			session['selected_case'] = case_name	
			return render_template('case/case.html', case_data = case_data)
		
		except Exception as e:
			print "Exception: " + str(e)
			resp['result_type'] = "danger"
			resp['result'] = "Specified case name is wrong!"
			resp['cases'] = session['cases']
			return render_template('accounts/index.html', resp = resp)

@app.route('/select_case', methods = ['GET', 'POST'])
def select_case():
	resp = {}
	resp['cases'] = session['cases']
	return render_template('accounts/index.html', resp = resp)
	
@app.route('/new_case', methods = ['GET', 'POST'])
def new_case():
	return render_template('accounts/new_case.html')

@app.route('/add_case', methods = ['GET', 'POST'])
def add_case():
	print "Add case"
	resp = {}
	if 'username' not in session:
		resp['result_type'] = "info"
		resp['result'] = "Please log in!"
		return render_template('accounts/login.html', resp = resp)
	if request.method == 'POST':
		temp = request.form['file_path']
		os_version = request.form['os_version']
		file_path = "../data/samples/" + temp

		# Adding this for automatic artifact extraction
		pids = []
		print "Spawned automation.py"
		pids.append(subprocess.Popen(["python", "automation.py", file_path, os_version]))
		
		os.system("g++ -std=c++11 ../mft.cpp -o ../mft -lWS2_32")
		command = "..\mft.exe " + file_path + " " + os_version
		print "Spawned mft"
		os.system("start /wait cmd /c " + command)

		# os.system is already a waiting call, but subprocesses is not, so this will wait
		# make both subprocesses in future
		for pid in pids:
			pid.wait()
		print "Both done"

		list_of_files = glob.glob('../data/json/*') # '*' means all if need specific format then '*.csv'
		json_file = os.path.basename(max(list_of_files, key=os.path.getctime))
		print "File added: " + json_file
						
		account_cases = []
		with open('../data/json/login.json', mode='r') as f:
			json_data = json.load(f)
			new_json_data = json_data
			i = 0
			for account in json_data['accounts']:
				if(account['username'] == session['username']):
					new_json_data['accounts'][i]['cases'].append(json_file)
					account_cases = new_json_data['accounts'][i]['cases']
				i += 1

		with open('../data/json/login.json', mode='w') as f:
			json.dump(new_json_data, f, indent = 4)

		resp['result_type'] = "success"
		resp['result'] = "Successfully added case!"
		resp['cases'] = account_cases
		session['cases'] = account_cases
		try:
			case_data = json.load(open('../data/json/' + json_file))
			new_case_data = case_data
		except Exception as e:
			print "Could not open case file"
			return render_template('accounts/index.html', resp = resp)
		
		# add code to read the automatic analysis file and add all the artifacts
		try:
			auto_data = json.load(open('../data/automatic_analysis.json'))
		except Exception as e:
			print "Could not open automatic analysis file"
			return render_template('accounts/index.html', resp = resp)
		pid_lists = ["process_list", "network_list", "dll_object_list", "phandle_list"]
		pids = []
		for keys in auto_data:
			pids.append(str(keys))
		print pids
		i = 0
		auto_artifacts = {}
		for artifact in case_data['artifacts']:
			key = next(iter(artifact))
			value = artifact[key]
			if key not in pid_lists:
				i += 1
				continue
			temp_list = []
			j = 0
			for module in value:
				if str(module['pid']) in pids:
					new_case_data['artifacts'][i][key][j]['marked'] = "disabled"
					if key == "process_list":
						new_case_data['artifacts'][i][key][j]['comment'] = auto_data[str(module['pid'])]
					elif key == "network_list":
						new_case_data['artifacts'][i][key][j]['comment'] = "Network connection related to malicious process (" + auto_data[str(module['pid'])] + ")"
					elif key == "dll_object_list":
						new_case_data['artifacts'][i][key][j]['comment'] = "DLLs linked to malicious process (" + auto_data[str(module['pid'])] + ")"
					elif key == "phandle_list":
						new_case_data['artifacts'][i][key][j]['comment'] = "Handles (Mutants) related to malicious process (" + auto_data[str(module['pid'])] + ")"
						
					temp_list.append(new_case_data['artifacts'][i][key][j])				
				j += 1
			auto_artifacts[key] = temp_list
			i += 1

		new_case_data['auto_artifacts'] = auto_artifacts
		session['selected_case'] = json_file
		
		with open('../data/json/' + session['selected_case'], mode='w') as f:
			json.dump(new_case_data, f, indent = 4)

		return render_template('case/case.html', case_data = case_data)
