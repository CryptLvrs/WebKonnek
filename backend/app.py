import subprocess
import json
import os

class systeminfo:
    def __init__(self, path):
        self.path = path
        self.executable_path = os.path.join(os.path.dirname(self.path), "sys_info")

    def get_system_data(self):
        try:
            # Run the C program 
            result = subprocess.run([self.executable_path], capture_output=True, text=True, check=True)

            # C program prints a JSON-like string to stdout
            json_output = result.stdout.strip() # will review that 
            return json.loads(json_output)

        except subprocess.CalledProcessError:
            print(f"Error while running C program at '{self.executable_path}'.")
            return None

        except json.JSONDecodeError:
            print(f"Error while parsing JSON output from C program: {json.JSONDecodeError}")
            print(f"Raw output: {json_output}")
            return None

        except FileNotFoundError:
            print(f"Error: C executable not found at '{self.executable_path}'. Did you compile it?") # will be updated
            return None

