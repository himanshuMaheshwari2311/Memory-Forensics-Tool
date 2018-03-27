from project import app
from flask import render_template, request
from flask_wtf import FlaskForm

@app.route('/')
def start():
    return render_template('printer/index.html')


@app.route('/print', methods=['GET', 'POST'])
def printer():
    if request.method == 'POST':
        return render_template('printer/index.html')
    return render_template('printer/print.html')
