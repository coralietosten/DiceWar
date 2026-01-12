import asyncio
import websockets

WS = "ws://localhost:5678/ws"

async def proxy(client_ws):
    async with websockets.connect(WS) as server_ws:

        async def client_to_server():
            async for msg in client_ws:
                await server_ws.send(msg)

        async def server_to_client():
            async for msg in server_ws:
                await client_ws.send(msg)

        await asyncio.gather(
            client_to_server(),
            server_to_client()
        )

async def main():
    async with websockets.serve(proxy, "0.0.0.0", 8080):
        print("Proxy lancé sur 8080")
        await asyncio.Future()

asyncio.run(main())
