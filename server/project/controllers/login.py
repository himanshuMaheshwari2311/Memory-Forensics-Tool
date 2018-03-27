from project import app
from flask import render_template, request
from flask_wtf import FlaskForm

import json

@app.route('/login', methods = ['GET', 'POST'])
def login():
	return render_template('accounts/login.html')