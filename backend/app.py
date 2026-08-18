import subprocess
import json
import os
from flask import Flask, jsonify, render_template 
from .monitor_reader import SystemMonitor



app = Flask(__name__)

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
C_EXECUTABLE_PATH = os.path.join(CURRENT_DIR, "../src_c/sys_info") # tested and display json output

# New route for the root URL
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/stats')
def get_stats():
    monitor = SystemMonitor(C_EXECUTABLE_PATH) 
    system_data = monitor.get_metrics() 
    return jsonify(system_data)

if __name__ == '__main__': 
    app.run(port=2222, debug=True) 