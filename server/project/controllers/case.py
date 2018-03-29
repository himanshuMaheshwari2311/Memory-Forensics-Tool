from project import app
from flask import render_template, request
from flask_wtf import FlaskForm

import json

@app.route('/case', methods = ['GET', 'POST'])
def case():
    if 'username' not in session:
        resp = {}
        resp['result'] = "Please log in!"
	    return render_template('accounts/login.html', resp = resp)
    if request.method == 'GET':
        case_name = request.args.get('case_name')
        case_data = json.load(open('../data/json/' + case_name))
        return render_template('case/case.html', case_data = case_data)