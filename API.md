# API calls for Embedded system

## Sensor Unit to Control Unit

---

### POST /connect

- **Description**: Post Sensor Unit Id to receive connection info
- **Returns**: Connection status
- **Body**:

``` json
{
  "sensor_unit_id": "550e8400-e29b-41d4-a716-446655440000"
}
```

- **Response**:

  - `200 OK` - The sensor unit is connected

    ```json
    { "status": "connected" }
    ```

  - `200 OK`: The sensor unit is not yet connected

    ```json
    { "status": "pending" }
    ```

---

### GET /time

- **Description**: Returns current time as a unix timestamp
- **Response**:
  - `200 OK`: `{ "timestamp": 1726995600 }`

---

### POST /readings

- **Description**: Post a batch of sensor readings
- **Returns**: Current connection status
- **Body**:

```json
{
  "sensor_unit_id": "550e8400-e29b-41d4-a716-446655440000",
  "readings": [
    {
      "timestamp": 1726995600,
      "temperature": 22.5,
      "humidity": 45.2
    },
    {
      "timestamp": 1726995605,
      "temperature": 22.7,
      "humidity": 44.9
    }
  ]
}
```

- **Response**:

  - `200 OK` - The sensor unit is still connected

    ```json
    { "status": "connected" }
    ```

  - `200 OK`: The sensor unit has been disconnected

    ```json
    { "status": "disconnected" }
    ```

## Control Unit to Backend

### Authentication

All API requests must include a valid JWT token in the `Authorization` header.

#### Header Format

```http
Authorization: Bearer <your-jwt-token>
```

The JWT token must include the following claim:

```json
{
  "unit_id": "<your-control-unit-id>"
}
```

- unit_id is a required claim.
- It must correspond to a valid Control Unit ID registered in the system

#### Generate JWT

A helper program for generating a key and token is located in `controlunit/helpers/jwtgenerator`

---

### POST /api/v1/control-unit

- **Description**: Post batched sensor readings grouped by timestamp
- **Returns**: status and number of saved readings

- **Body**:

``` json
{
  "control_unit_id": "f47ac10b-58cc-4372-a567-0e02b2c3d479",
  "timestamp_groups": [
    {
      "timestamp": 1726995600,
      "sensor_units": [
        {
          "sensor_unit_id": "550e8400-e29b-41d4-a716-446655440000",
          "temperature": 22.5,
          "humidity": 45.2
        }
      ]
    },
    {
      "timestamp": 1726995605,
      "sensor_units": [
        {
          "sensor_unit_id": "550e8400-e29b-41d4-a716-446655440000",
          "temperature": 22.6,
          "humidity": 45.1
        }
      ]
    }
  ]
}
```

- **Response**:

  - `200 OK` - Succesfully received n readings

    ```json
    { 
      "status": "ok",
      "saved" : 2  
    }
    ```

---

### POST /api/v1/control-unit/status

- **Description**: Post Control Unit Id to receive connection info
- **Returns**: Connection status for one or more Sensor Units

- **Body**:

``` json
{
  "control_unit_id": "f47ac10b-58cc-4372-a567-0e02b2c3d479"
}
```

- **Response**:

  - `200 OK` - Simplified connect command for one unit

    ```json
    { 
      "sensor_unit_id": "550e8400-e29b-41d4-a716-446655440000",  
      "status": "in_transit"
    }
    ```

  - `200 OK` - Simplified disconnect command for one unit

    ```json
    { 
      "sensor_unit_id": "550e8400-e29b-41d4-a716-446655440000",
      "status": "delivered"
    }
    ```

  *Future improved version*
- `200 OK` - Connect/disconnect commands for several units

    ```json
    [ 
      {
          "sensor_unit_id": "550e8400-e29b-41d4-a716-446655440000",
          "status": "connected"
      },
      {
          "sensor_unit_id": "123e4567-e89b-12d3-a456-426614174000",
          "status": "disconnected"
      } 
    ]
    ```
