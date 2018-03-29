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
	resp = {}
	if request.method == 'POST':
		account = credentials(request.form['username'], request.form['password'])
		if(account == "pwd"):
			resp['result'] = "Incorrect pwd!"
			print resp['result']
			return render_template('accounts/login.html', resp = resp)
		elif (account == "uname"):
			resp['result'] = "Invalid uname!"
			print resp['result']
			return render_template('accounts/login.html', resp = resp)
		else:
			resp['result'] = "Logged in!"
			print resp['result']
			resp['cases'] = account['cases']
			session['username'] = request.form['username']
			return render_template('accounts/index.html', resp = resp)
	resp['result'] = ""
	return render_template('accounts/login.html', resp = resp)

@app.route('/logout', methods = ['GET', 'POST'])
def logout():
	if 'username' in session:
		session.pop('username')
	resp = {}
	resp['result'] = "Successfully logged out!"
	return render_template('accounts/login.html', resp = resp)