from project import app
from flask import render_template, request, session, redirect, url_for
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


@app.route('/')
def start():
	return redirect(url_for('login'))

@app.route('/login', methods = ['GET', 'POST'])
def login():
	resp = {}
	if 'username' in session:
		resp['result_type'] = "info"
		resp['result'] = "Already logged in!"
		resp['cases'] = session['cases']
		return render_template('accounts/index.html', resp = resp)
	if request.method == 'POST':
		account = credentials(request.form['username'], request.form['password'])
		if(account == "pwd"):
			resp['result_type'] = "danger"
			resp['result'] = "Incorrect password!"
			return render_template('accounts/login.html', resp = resp)
		elif (account == "uname"):
			resp['result_type'] = "danger"
			resp['result'] = "Invalid username!"
			return render_template('accounts/login.html', resp = resp)
		else:
			resp['result_type'] = "success"
			resp['result'] = "Successfully logged in!"
			resp['cases'] = account['cases']
			session['username'] = request.form['username']	
			session['cases'] = account['cases']
			return render_template('accounts/index.html', resp = resp)
	resp['result_type'] = "info"
	resp['result'] = "Log in to continue"
	return render_template('accounts/login.html', resp = resp)

@app.route('/logout', methods = ['GET', 'POST'])
def logout():
	if 'username' in session:
		session.pop('username')
	resp = {}
	resp['result_type'] = "info"
	resp['result'] = "Successfully logged out!"
	return render_template('accounts/login.html', resp = resp)

@app.route('/register')
def register():
	return render_template('accounts/register.html')