C_SOURCE = src_c/sys_info.c
C_BINARY = src_c/sys_info
VENV = .venv
PYTHON = $(VENV)/bin/python
FLASK = $(VENV)/bin/flask
PORT = 2222

.PHONY: all run clean password

all: $(C_BINARY) $(PYTHON)
	@echo "Everything is ready. Run 'make run'."

$(C_BINARY): $(C_SOURCE)
	@echo "Compiling the C program..."
	gcc $(C_SOURCE) -o $(C_BINARY)

$(VENV)/bin/python:
	@echo "Creating the Python virtual environment..."
	python3 -m venv $(VENV)
	$(VENV)/bin/pip install -r requirements.txt

run: all
	@echo "Starting the server on port $(PORT)..."
	@echo "Default password is 'admin' unless WEBKONNEK_PASSWORD is set."
	WEBKONNEK_PASSWORD=$${WEBKONNEK_PASSWORD:-admin} $(FLASK) --app backend.app run --port $(PORT)

password:
	@echo "Your new password:"
	@python3 -c "import secrets; print(secrets.token_urlsafe(16))"

clean:
	rm -rf $(VENV) $(C_BINARY)
	find . -type d -name __pycache__ -exec rm -rf {} +
	@echo "Cleaned up."