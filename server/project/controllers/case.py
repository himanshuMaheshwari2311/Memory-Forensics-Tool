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
			case_data = json.load(open('../data/json/' + case_name))
			return render_template('case/case.html', case_data = case_data)
		except:
			resp['result_type'] = "danger"
			resp['result'] = "Specified case name is wrong!"
			resp['cases'] = session['cases']
			return render_template('accounts/index.html', resp = resp)