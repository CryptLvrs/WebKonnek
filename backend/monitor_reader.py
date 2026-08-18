import subprocess
import json

class SystemMonitor:
    def __init__(self, path):
        self.executable_path = path

    def get_metrics(self):
        try:
            result = subprocess.run(
                [self.executable_path],
                capture_output=True,
                text=True,
                check=True
            )
            # Parse the JSON output from the C program's stdout
            return json.loads(result.stdout.strip())

        except (subprocess.CalledProcessError, json.JSONDecodeError, FileNotFoundError) as err:
            # Handle errors during execution or JSON parsing (past error processing wasn't the best solution)
            error_message = f"Error processing system info: {err}"
            if hasattr(err, 'stderr') and err.stderr:
                error_message += f" | C program output: {err.stderr.strip()}"
            return {"error": error_message}