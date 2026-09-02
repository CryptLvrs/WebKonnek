import os
import shutil
from flask import Flask, jsonify, render_template, session, redirect, url_for, request
from werkzeug.security import generate_password_hash, check_password_hash
from .monitor_reader import SystemMonitor



app = Flask(__name__)
app.secret_key = os.environ.get("WEBKONNEK_SECRET_KEY", "dev-secret-change-me")

PASSWORD_HASH = generate_password_hash(os.environ.get("WEBKONNEK_PASSWORD", "admin"))


def login_required(func):
    def wrapper(*args, **kwargs):
        if not session.get("logged_in"):
            if request.path.startswith("/api"):
                return jsonify({"error": "Authentication required"}), 401 # needs a '401 Unauthorized' otherwise it won't work 
            return redirect(url_for("login"))
        return func(*args, **kwargs)
    wrapper.__name__ = func.__name__ # without that it's seem to "collide" routes 
    return wrapper


CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
C_EXECUTABLE_PATH = os.path.join(CURRENT_DIR, "../src_c/sys_info")

# route for the root URL
@app.route('/')
@login_required
def index():
    return render_template('index.html')

# !!!!!!!!!!!!!!!
# THIS IS FINE ON 'localhost' BUT NOT FINE ON LAN!!!
# !!!!!!!!!!!!!!!
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        password = request.form.get('password', '')
        if check_password_hash(PASSWORD_HASH, password):
            session['logged_in'] = True
            return redirect(url_for('index'))
        return render_template('login.html', error='Invalid password')
    return render_template('login.html')

@app.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('login'))

@app.route('/api/stats')
@login_required
def get_stats():
    monitor = SystemMonitor(C_EXECUTABLE_PATH)
    system_data = monitor.get_metrics()

    disk = shutil.disk_usage("/")
    system_data["disk_total"] = disk.total
    system_data["disk_free"] = disk.free

    return jsonify(system_data)

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True) 

# COMMANDS SECTIONS FOR DEV
# source .venv/bin/activate
# THEN
# WEBKONNEK_PASSWORD=the-password python -m backend.app 
# ELSE
# WEBKONNEK_PASSWORD=the-password python -m flask --app backend.app run --port 2222 
# (for me the first one command work if for you that don't work, do the second command)