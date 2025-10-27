# JWT generator

## Option 1 - run with Makefile

`cd helpers/jwtgenerator`  
`make key`  
`make token`  

## Option 2 - run the commands manually

1. Navigate to this directory:  
`cd helpers/jwtgenerator`  

2. First create a secret signing key:  
`openssl rand 64 | base64 | tr -d '\n' > secret.key`  

3. Make sure `secret.key` is included in `.gitignore` and never pushed to GitHub  

4. Then you can generate a JWT by running:  
`go run jwt_generator.go`  
