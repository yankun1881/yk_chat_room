## USE my_database;
# 用户表
CREATE TABLE users ( 
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(25) NOT NULL,
    status TINYINT NOT NULL,
    password VARCHAR(100) NOT NULL ); 
状态>1 为有效

# 好友表
CREATE TABLE friends (
    user_id1 INT NOT NULL,
    user_id2 INT NOT NULL,
    status TINYINT NOT NULL,
    PRIMARY KEY (user_id1, user_id2),
    FOREIGN KEY (user_id1) REFERENCES users(id),
    FOREIGN KEY (user_id2) REFERENCES users(id)
);


# 群组表
CREATE TABLE group_table(
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
# 用户群组表
CREATE TABLE user_group(
user_id INT,
    group_id INT,
    PRIMARY KEY (user_id, group_id),
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (group_id) REFERENCES group_table(id)
);

# 消息记录表
CREATE TABLE messages (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    message VARCHAR(2000) NOT NULL,
    send_time DATETIME NOT NULL, -- 存储消息发送时间
    INDEX idx_send_time (send_time) -- 在 send_time 上创建索引
) ENGINE=InnoDB;