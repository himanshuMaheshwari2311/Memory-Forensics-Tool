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
