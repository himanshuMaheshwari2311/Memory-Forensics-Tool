from project import app
from flask import render_template, request
from flask_wtf import FlaskForm

@app.route('/login')
def login():
    return render_template('login/login.html')