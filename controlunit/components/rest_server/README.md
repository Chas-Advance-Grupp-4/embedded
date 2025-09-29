# REST Server Component for ESP32-S3-Zero

This component sets up a lightweight RESTful HTTP server using ESP-IDF's built-in `esp_http_server`. It is just an example with basic GET and POST requests.

## Endpoints

`GET /hello`  
`GET /status`  
`POST /postexample`  

To test the POST endpoint using `curl`:

```bash
curl -X POST http://<ESP32-IP>/postexample \
     -H "Content-Type: text/plain" \
     -d 'Your message here!'
```

The ESP32 IP will be printed in the log when you run  
`idf.py -p /dev/tty.usbmodem1101 monitor`  
*The modem address `/dev/tty.usbmodem1101` might be different on your machine*
