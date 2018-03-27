from project import app
from flask import render_template, request, session, flash
from flask_wtf import FlaskForm

import os
import json

def credentials(uname, pwd):
	with open('../data/json/login.json') as json_file:
		json_data = json.load(json_file)
		for account in json_data['accounts']:
			if(account['username'] == uname):
				if(account['password'] == pwd):
					return account
				else:
					return "pwd"
		return "uname"

@app.route('/login', methods = ['GET', 'POST'])
def login():
	print os.getcwd()
	
	if request.method == 'POST':
		account = credentials(request.form['username'], request.form['password'])
		resp = {}
		if(account == "pwd"):
			resp['result'] = "Incorrect pwd!"
			print resp['result']
		elif (account == "uname"):
			resp['result'] = "Invalid uname!"
			print resp['result']
		else:
			resp['result'] = "Logged in!"
			print resp['result']
			resp['cases'] = account['cases']

		session['username'] = request.form['username']
		return render_template('accounts/index.html', **resp)
	return render_template('accounts/login.html')