import threading
import time
import queue # 使用佇列作為有界緩衝區

# 有界緩衝區大小
BUFFER_SIZE = 5

# 共享的有界緩衝區
buffer = queue.Queue(BUFFER_SIZE)

# 生產者函數
def producer():
    for i in range(10):
        item = f"Item {i}"
        # 嘗試在緩衝區中新增數據，如果緩衝區已滿則等待
        buffer.put(item)
        print(f"Produced {item}")
        time.sleep(1)

# 消費者函數
def consumer():
    while True:
        # 嘗試從緩衝區中取出數據，如果緩衝區為空則等待
        item = buffer.get()
        print(f"Consumed {item}")
        time.sleep(1)

# 創建生產者和消費者線程
producer_thread = threading.Thread(target=producer)
consumer_thread = threading.Thread(target=consumer)

# 啟動執行緒
producer_thread.start()
consumer_thread.start()

# 等待執行緒結束
producer_thread.join()
consumer_thread.join()
