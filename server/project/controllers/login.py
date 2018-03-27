from project import app
from flask import render_template, request
from flask_wtf import FlaskForm

@app.route('/login', methods = ['GET', 'POST'])
def login():
	return render_template('accounts/login.html')
	if request.method == 'POST':
		session['username'] = request.form['username']
		return render_template('login/index.html')
    return render_template('printer/index.html')