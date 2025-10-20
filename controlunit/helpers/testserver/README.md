# This program will set up a local server duplicating the Chas Advance IoT control unit

To run the program:  

```bash
go run caserver.go
```

To post data send a HTTP Post to
`http://localhost:8080/post`  

To access the result open a browser and go to  
`http://localhost:8080`  

To post using curl
`curl -X POST -H "Content-Type: application/json" -d '{"content":"Hello Lets Go"}' http://localhost:8080/post`
