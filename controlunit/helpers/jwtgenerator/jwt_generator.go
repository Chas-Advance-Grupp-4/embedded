package main

import (
	"fmt"
	"os"
	"time"

	"github.com/golang-jwt/jwt/v5"
)

func main() {
	// Your secret key
	secret, err := os.ReadFile("secret.key")
	if err != nil {
		panic(err)
	}

	// Create claims
	claims := jwt.MapClaims{
		"sub":  "controlunits",
		"role": "sensorgateway",
		"iat":  time.Now().Unix(),
		"exp":  time.Now().Add(30 * time.Minute).Unix(),
	}

	// Create token
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)

	// Sign token
	signedToken, err := token.SignedString(secret)
	if err != nil {
		panic(err)
	}

	// Print token to console
	fmt.Println("JWT Token:")
	fmt.Println(signedToken)
}
