package main

import (
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/golang-jwt/jwt/v5"
)

func main() {
	// Your secret key
	secret, err := os.ReadFile("secret.key")
	if err != nil {
		panic(err)
	}

	unitIdBytes, err := os.ReadFile("control_unit_id")
	if err != nil {
		panic(err)
	}
	unitId := strings.TrimSpace(string(unitIdBytes)) // Ingen base64!

	// Create claims
	claims := jwt.MapClaims{
		"sub":     "controlunits",
		"role":    "sensorgateway",
		"unit_id": unitId,
		"iat":     time.Now().Unix(),
		"exp":     time.Now().Add(365 * 24 * time.Hour).Unix(),
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
