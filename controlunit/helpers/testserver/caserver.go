package main

import (
	"encoding/json"
	"fmt"
	"html/template"
	"io"
	"log"
	"net/http"
	"sync"
	"time"
)

// Structar för readings
type SensorUnit struct {
	UUID        string  `json:"sensor_unit_id"`
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
}

type TimestampGroup struct {
	Timestamp   int64        `json:"timestamp"`
	SensorUnits []SensorUnit `json:"sensor_units"`
}

type Payload struct {
	DeviceUUID      string           `json:"control_unit_id"`
	TimestampGroups []TimestampGroup `json:"timestamp_groups"`
}

var (
	senastePayload Payload
	mu             sync.Mutex
)

type FormattedSensorUnit struct {
	UUID        string
	Temperature string
	Humidity    string
}

type FormattedTimestampGroup struct {
	Timestamp   string
	SensorUnits []FormattedSensorUnit
}

type FormattedPayload struct {
	DeviceUUID      string
	TimestampGroups []FormattedTimestampGroup
}

func main() {
	http.HandleFunc("/", visaHTML)
	http.HandleFunc("/post", taEmotPost)
	fmt.Println("POST to /post")
	fmt.Println("Server runs on http://0.0.0.0:8080...")
	log.Fatal(http.ListenAndServe("0.0.0.0:8080", nil))
}

func visaHTML(w http.ResponseWriter, r *http.Request) {
	mu.Lock()
	defer mu.Unlock()

	// Format payload
	var formattedGroups []FormattedTimestampGroup
	for _, group := range senastePayload.TimestampGroups {
		formattedTime := time.Unix(group.Timestamp, 0).UTC().Format(time.RFC3339)

		var formattedUnits []FormattedSensorUnit
		for _, unit := range group.SensorUnits {
			formattedUnits = append(formattedUnits, FormattedSensorUnit{
				UUID:        unit.UUID,
				Temperature: fmt.Sprintf("%.2f", unit.Temperature),
				Humidity:    fmt.Sprintf("%.1f", unit.Humidity),
			})
		}

		formattedGroups = append(formattedGroups, FormattedTimestampGroup{
			Timestamp:   formattedTime,
			SensorUnits: formattedUnits,
		})
	}

	formattedPayload := FormattedPayload{
		DeviceUUID:      senastePayload.DeviceUUID,
		TimestampGroups: formattedGroups,
	}

	// HTML-template
	tmpl := `<html>
<head>
    <title>Chas Advance Control Unit Testserver</title>
</head>
<body>
    <h1>Chas Advance Control Unit Testserver</h1>
    <h2>Latest readings</h2>
    <p><strong>Device UUID:</strong> {{.DeviceUUID}}</p>
    {{if .TimestampGroups}}
        {{range .TimestampGroups}}
            <h3>Timestamp: {{.Timestamp}}</h3>
            <ul>
                {{range .SensorUnits}}
                    <li>{{.UUID}} — Temp: {{.Temperature}}, Humidity: {{.Humidity}}%</li>
                {{end}}
            </ul>
        {{end}}
    {{else}}
        <p><em>timestamp_groups: []</em></p>
    {{end}}
</body>
</html>`

	t := template.Must(template.New("webpage").Parse(tmpl))
	t.Execute(w, formattedPayload)
}

func taEmotPost(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST request supported", http.StatusMethodNotAllowed)
		return
	}

	body, err := io.ReadAll(r.Body)
	if err != nil {
		http.Error(w, "Could not read request", http.StatusBadRequest)
		return
	}
	defer r.Body.Close()

	var nyttPayload Payload
	err = json.Unmarshal(body, &nyttPayload)
	if err != nil {
		http.Error(w, "Invalid JSON format", http.StatusBadRequest)
		return
	}

	mu.Lock()
	senastePayload = nyttPayload
	mu.Unlock()

	fmt.Fprintln(w, "Readings received!")
}
