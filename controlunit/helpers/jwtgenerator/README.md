# JWT generator

## Option 1 - run with Makefile

- `cd helpers/jwtgenerator`  
- `make id`  
- paste your Control Unit Id into file `control_unit_id`  
- `make key`  
- `make token`  

## Option 2 - run the commands manually

1. Navigate to this directory:  
`cd helpers/jwtgenerator`  

2. Create a file called `control_unit_id` and paste your Control Unit Id here

3. First create a secret signing key:  
`openssl rand 64 | base64 | tr -d '\n' > secret.key`  

4. Make sure `secret.key` is included in `.gitignore` and never pushed to GitHub  

5. Then you can generate a JWT by running:  
`go run jwt_generator.go`  
