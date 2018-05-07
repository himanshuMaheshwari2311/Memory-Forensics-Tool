from project import app
from flask import render_template, request, session, send_from_directory
from flask_wtf import FlaskForm
import os
import json

# add function
@app.route('/add_artifact', methods = ['GET', 'POST'])
def add_artifact():
    if request.method == 'POST':
		object_id = request.form['object_id']
		resp = {}
		with open('../data/json/' + session['selected_case'], mode='r') as f:
			case_data = json.load(f)
			new_case_data = case_data
			i = 0
			for artifact in case_data['artifacts']:
				key = next(iter(artifact))
				value = artifact[key]
				j = 0
				for module in value:
					if str(module['object_id']) == str(object_id) and module['marked'] == "enabled":
						new_case_data['artifacts'][i][key][j]['marked'] = "disabled"
						if 'comment' in request.form:
							new_case_data['artifacts'][i][key][j]['comment'] = request.form['comment']
						resp['obj'] = new_case_data['artifacts'][i][key][j]
					j += 1
				i += 1
		with open('../data/json/' + session['selected_case'], mode='w') as f:
			json.dump(new_case_data, f, indent = 4)

		resp['result'] = True
		return json.dumps(resp)

# remove function
@app.route('/remove_artifact', methods = ['GET', 'POST'])
def remove_artifact():
    if request.method == 'POST':
		object_id = request.form['object_id']
		resp = {}
		with open('../data/json/' + session['selected_case'], mode='r') as f:
			case_data = json.load(f)
			new_case_data = case_data
			i = 0
			for artifact in case_data['artifacts']:
				key = next(iter(artifact))
				value = artifact[key]
				j = 0
				for module in value:
					if str(module['object_id']) == str(object_id) and module['marked'] == "disabled":
						new_case_data['artifacts'][i][key][j]['marked'] = "enabled"
						if 'comment' in new_case_data['artifacts'][i][key][j]:
							new_case_data['artifacts'][i][key][j]['comment'] = ""
					j += 1
				i += 1
		with open('../data/json/' + session['selected_case'], mode='w') as f:
			json.dump(new_case_data, f, indent = 4)

		resp['result'] = True
		return json.dumps(resp)