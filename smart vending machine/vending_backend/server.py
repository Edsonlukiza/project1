from fastapi import FastAPI, WebSocket, WebSocketDisconnect
import uvicorn

app = FastAPI()
connected_machine = None

# 1. This fixes the "Not Found" error in your browser
@app.get("/")
async def root():
    status = "CONNECTED" if connected_machine else "DISCONNECTED"
    return {
        "message": "Vending Machine Backend is Running",
        "machine_status": status
    }

# 2. The WebSocket route
@app.websocket("/ws/vending01")
async def websocket_endpoint(websocket: WebSocket):
    global connected_machine
    await websocket.accept()
    connected_machine = websocket
    print("✅ SUCCESS: ESP32 has connected to the server!")
    try:
        while True:
            # Wait for any data to keep the connection alive
            await websocket.receive_text()
    except WebSocketDisconnect:
        print("❌ ALERT: ESP32 has disconnected.")
        connected_machine = None

# 3. The Payment Trigger
@app.get("/pay")
async def simulate_payment():
    if connected_machine:
        await connected_machine.send_json({"action": "dispense"})
        return {"status": "Success", "message": "Dispense command sent!"}
    return {"status": "Error", "message": "Machine not connected"}

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)