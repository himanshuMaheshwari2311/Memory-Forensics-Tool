from project import app
from flask import render_template, request, session
from flask_wtf import FlaskForm

import json

@app.route('/case', methods = ['GET', 'POST'])
def case():
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
			with open('../data/json/report_' + case_name, mode = 'w') as f:
				json.dump([], f)
			case_data = json.load(open('../data/json/' + case_name))
			session['selected_case'] = case_name
			
			return render_template('case/case.html', case_data = case_data)
		except Exception as e:
			print "Exception: " + str(e)
			resp['result_type'] = "danger"
			resp['result'] = "Specified case name is wrong!"
			resp['cases'] = session['cases']
			return render_template('accounts/index.html', resp = resp)

# add function
@app.route('/add_artifact', methods = ['GET', 'POST'])
def add_to_report():
    if request.method == 'POST':
		report_data = {}
		object_id = request.form['object_id']
		with open('../data/json/report_' + session['selectedd_case'], mode='w') as f:
			case_data = json.load(f)
		for artifact in case_data['artifacts']:
			for key, value in artifacts.iteritems():
				for module in value:
					if module['object_id'] == object_id and module['marked'] == "enabled":
						module['marked'] = "disabled"
						report_data[object_id] = [ key, module ]
		json.dump(case_data, f)
		with open('../data/json/report_' + session['selectedd_case'], mode='w') as report_json:
			report = json.load(report_json)
			report.append(report_data)
			json.dump(report, report_json)

