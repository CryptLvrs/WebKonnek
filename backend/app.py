import subprocess
import json
import os
from flask import Flask, jsonify

class systeminfo:
    def __init__(self, path):
        self.executable_path = path

    def get_system_data(self):
        try: 
            result = subprocess.run([self.executable_path], capture_output=True, text=True, check=True)

            json_output = result.stdout.strip() # will review that 
            return json.loads(json_output)

        except (subprocess.CalledProcessError, json.JSONDecodeError, FileNotFoundError) as err:
            # I emplement log errors over printing them
            # The C program's stderr is in e.stderr if it's a CalledProcessError and I will see if that way is better
            error_message = f"Error processing system info: {err}"
            if hasattr(err, 'stderr') and err.stderr:
                error_message += f" | C program output: {err.stderr.strip()}"
            return {"error": error_message}

app = Flask(__name__)

C_EXECUTABLE_PATH = "../src_c/sys_info" # will review that

@app.route('/api/stats')
def get_stats():
    monitor = systeminfo(C_EXECUTABLE_PATH)
    system_data = monitor.get_system_data()
    return jsonify(system_data)

if __name__ == '__main__':
    app.run(debug=True) # (Temporary) allows to run the app directly with "python app.py" 