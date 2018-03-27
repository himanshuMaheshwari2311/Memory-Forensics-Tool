from project import app
from flask import render_template, request
from flask_wtf import FlaskForm

import json

@app.route('/case', methods = ['GET', 'POST'])
def open_case():
    if request.method == 'GET':
        case_name = request.args.get('case_name')
        json.load(open('../data/json/' + case_name))
        return render_template('case/case.html', **json)