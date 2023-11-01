from flask import Flask, render_template, request, jsonify

app = Flask(__name)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/check_flood', methods=['POST'])
def check_flood():
    # Extract the PIN code from the POST request
    pin_code = request.form['pinCode']

    # Replace this logic with your data retrieval and processing
    # For this example, we return a static response based on the PIN code.
    if pin_code == '607004':
        response_data = {'message': 'No flood alerts for PIN code 607004.'}
    else:
        response_data = {'message': 'Flood alert for PIN code ' + pin_code + '!'}

    return jsonify(response_data)

if __name__ == '__main__':
    app.run(debug=True)
