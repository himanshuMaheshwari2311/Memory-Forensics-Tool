# A very simple Flask Hello World app for you to get started with...

from flask import Flask
from flask_restful import Resource, Api, reqparse
from flask_cors import CORS

print "Start"

app = Flask(__name__)
CORS(app)

test = [{'number': 14, 'name': 'Kakuna'},
           {'number': 16, 'name': 'Pidgey'},
           {'number': 50, 'name': 'Diglett'}]


class MyApp(Resource):
    def get(self):
        return test

    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('query', type=str, required=True, location='json')
        args = parser.parse_args(strict=True)
        ans = args['query']
        return ans

api = Api(app)
api.add_resource(MyApp, '/api/v1/analyze')

@app.route('/')
def home():
    return "Home"

@app.route('/hello')
def hello():
    return "Hello"

app.run()
print "End"