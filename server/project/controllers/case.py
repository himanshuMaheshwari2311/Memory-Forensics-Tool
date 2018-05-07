from project import app
from flask import render_template, request, session, send_from_directory
from flask_wtf import FlaskForm
import os
import json
import glob

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

@app.route('/add_case', methods = ['GET', 'POST'])
def add_case():
	print "Add case"
	resp = {}
	if 'username' not in session:
		resp['result_type'] = "info"
		resp['result'] = "Please log in!"
		return render_template('accounts/login.html', resp = resp)
	if request.method == 'POST':
		file_path = request.form['file_path']
		os_version = request.form['os_version']
		
		#os.system("g++ -std=c++11 ../mft.cpp -o ../mft")
		
		command = "..\mft.exe " + file_path + " " + os_version
		os.system("start /wait cmd /c " + command)

		list_of_files = glob.glob('../data/json/*') # * means all if need specific format then *.csv
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
		session['username'] = request.form['username']	
		session['cases'] = account_cases
		try:
			case_data = json.load(open('../data/json/' + json_file))
		except Exception as e:
			print "Could not open file"
			return render_template('accounts/index.html', resp = resp)
		session['selected_case'] = json_file
		return render_template('case/case.html', case_data = case_data)