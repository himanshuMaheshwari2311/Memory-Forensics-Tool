from project import app
from flask import render_template, request, session, send_from_directory
from flask_wtf import FlaskForm

import json

@app.route('/send_report', methods = ['GET', 'POST'])
def send_report():
	try:
		return send_from_directory('..\\..\\data\\pdfs', 'cl.pdf')
	except Exception as e:
		return str(e)

@app.route('/update_report', methods = ['GET', 'POST'])
def update_report():
	if request.method == 'POST':
		overview = request.form['case_overview']
		acquisition = request.form['case_acquisition']
		findings = request.form['case_findings']
		conclusion = request.form['case_conclusion']
		
		resp = {}
		case_data = {}
		
		with open('../data/json/' + session['selected_case'], mode='r') as f:
			case_data = json.load(f)
			case_data['case_overview'] = overview
			case_data['case_acquisition'] = acquisition
			case_data['case_findings'] = findings
			case_data['case_conclusion'] = conclusion
		with open('../data/json/' + session['selected_case'], mode='w') as f:
			json.dump(case_data, f, indent = 4)

		resp['result'] = True
		return json.dumps(resp)