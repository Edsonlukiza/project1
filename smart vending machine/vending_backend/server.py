from fastapi import FastAPI, WebSocket
import uvicorn

app = FastAPI()

@app.websocket("/ws/vending01")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    print("Vending Machine Connected!")
    try:
        while True:
            # In a real project, this triggers after an M-Pesa Webhook
            cmd = input("Type 'pay' to dispense: ")
            if cmd == "pay":
                await websocket.send_json({"action": "dispense"})
    except Exception as e:
        print(f"Disconnected: {e}")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)