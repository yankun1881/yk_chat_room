// service-worker.js

self.addEventListener('fetch', function(event) {
    // 拦截所有 WebSocket 连接请求
    if (event.request.url.startsWith('ws://')) {
        event.respondWith(handleWebSocketRequest(event.request));
    }
});

async function handleWebSocketRequest(request) {
    // 使用 fetch API 来发起 WebSocket 连接
    const response = await fetch(request);

    // 返回 WebSocket 连接响应
    return response;
}
