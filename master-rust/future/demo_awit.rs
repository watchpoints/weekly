use std::future::Future;
use std::pin::Pin;
use std::task::{Context, Poll};
use std::time::Duration;

// 一个简单的异步函数，模拟一个延时操作
async fn delay(duration: Duration) {
    println!("Sleeping for {:?}", duration);
    // 使用 tokio 的 sleep 函数来模拟异步延时
    tokio::time::sleep(duration).await;
    println!("Woke up after {:?}", duration);
}

// 另一个异步函数，调用 delay 函数并等待它完成
async fn perform_task() {
    println!("Task started");
    delay(Duration::from_secs(2)).await; // 等待 2 秒
    println!("Task completed");
}

#[tokio::main]
async fn main() {
    println!("Main started");
    perform_task().await; // 等待 perform_task 完成
    println!("Main completed");
}