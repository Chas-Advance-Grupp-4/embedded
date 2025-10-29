# Convenience for generating JWT token
.PHONY: help id key token 

.DEFAULT_GOAL := help

help:
	@echo "Available commands:"
	@echo "  make id          - Generate the empty file control_unit_id. Paste your Control Unit Id into it"
	@echo "  make key         - Generate a Signing Key in the file secret.key"
	@echo "  make token       - Generate a JWT token in the terminal to be copied to secret config file"

id:
	@printf "00000000-0000-0000-0000-000000000000" > control_unit_id
	@echo "control_unit_id created. Paste your Control Unit Id into the file"

# Generate a secret.key file
key:
	openssl rand 64 | base64 | tr -d '\n' > secret.key

# Generate a JWT token from the secret key
token:
	go run jwt_generator.go
