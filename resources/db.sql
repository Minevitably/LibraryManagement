-- 创建数据库
CREATE
DATABASE IF NOT EXISTS library_management
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;
USE
library_management;

-- 创建用户表
CREATE TABLE users
(
    id         INT AUTO_INCREMENT PRIMARY KEY,
    username   VARCHAR(50)  NOT NULL UNIQUE,
    password   VARCHAR(255) NOT NULL, -- 存储加密后的密码
    name       VARCHAR(100) NOT NULL,
    user_type  ENUM('admin', 'reader') NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- 创建扩展版图书表
CREATE TABLE books
(
    id           INT AUTO_INCREMENT PRIMARY KEY,
    title        VARCHAR(255) NOT NULL,          -- 图书名
    author       VARCHAR(100) NOT NULL,          -- 作者
    isbn         VARCHAR(20) UNIQUE,             -- ISBN号，唯一
    publisher    VARCHAR(100),                   -- 出版社
    description  TEXT,                           -- 图书简介
    language     VARCHAR(30) DEFAULT '中文',     -- 语言，默认为中文
    publish_date DATE,                           -- 出版日期
    quantity     INT UNSIGNED DEFAULT 0,         -- 库存数量，无符号整数
    created_at   TIMESTAMP   DEFAULT CURRENT_TIMESTAMP,
    updated_at   TIMESTAMP   DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX        idx_title (title),              -- 图书名索引
    INDEX        idx_author (author),            -- 作者索引
    INDEX        idx_publisher (publisher),      -- 出版社索引
    INDEX        idx_isbn (isbn),                -- ISBN索引
    INDEX        idx_publish_date (publish_date) -- 出版日期索引
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;


-- 插入管理员用户
INSERT INTO users (username, password, name, user_type)
VALUES ('admin', '123456', '系统管理员', 'admin');

-- 插入普通读者用户
INSERT INTO users (username, password, name, user_type)
VALUES ('reader1', '123456', '张三', 'reader'),
       ('reader2', '123456', '李四', 'reader');


-- 插入20条图书记录
INSERT INTO books (title, author, isbn, publisher, description, language,
                   publish_date, quantity)
VALUES ('三体', '刘慈欣', '9787536692930', '重庆出版社',
        '中国科幻文学的里程碑之作，讲述了地球文明与三体文明的接触与冲突。',
        '中文', '2008-01-01', 15),
       ('活着', '余华', '9787506365437', '作家出版社',
        '讲述一个人在中国社会变革中的苦难人生，展现生命的坚韧与价值。', '中文',
        '1993-06-01', 20),
       ('百年孤独', '加西亚·马尔克斯', '9787544258974', '南海出版公司',
        '魔幻现实主义文学代表作，讲述布恩迪亚家族七代人的传奇故事。', '中文',
        '2011-06-01', 12),
       ('小王子', '安托万·德·圣-埃克苏佩里', '9787532776774', '人民文学出版社',
        '一部写给大人的童话，探讨爱与责任、孤独与友谊的人生哲理。', '中文',
        '2013-01-01', 25),
       ('围城', '钱钟书', '9787020090006', '人民文学出版社',
        '中国现代文学经典，描写抗战初期知识分子的群像。', '中文', '1947-05-01',
        18),
       ('白夜行', '东野圭吾', '9787544258609', '南海出版公司',
        '日本推理小说代表作，讲述两个命运交织的孩子的悲剧故事。', '中文',
        '2013-01-01', 10),
       ('人类简史', '尤瓦尔·赫拉利', '9787508647357', '中信出版社',
        '从认知革命到科学革命，重新审视人类历史的进程。', '中文', '2014-11-01',
        8),
       ('Python编程：从入门到实践', 'Eric Matthes', '9787115428028',
        '人民邮电出版社', 'Python编程入门经典教程，适合零基础学习者。', '中文',
        '2016-07-01', 30),
       ('时间简史', '史蒂芬·霍金', '9787535732309', '湖南科学技术出版社',
        '介绍宇宙本性的科普著作，探索时空和宇宙的奥秘。', '中文', '2010-04-01',
        5),
       ('追风筝的人', '卡勒德·胡赛尼', '9787208061644', '上海人民出版社',
        '讲述阿富汗两个少年之间关于友谊、背叛与救赎的故事。', '中文',
        '2006-05-01', 22),
       ('明朝那些事儿', '当年明月', '9787801656087', '中国友谊出版公司',
        '用现代语言讲述明朝历史的通俗读物，兼具趣味性与知识性。', '中文',
        '2006-09-01', 28),
       ('解忧杂货店', '东野圭吾', '9787544270878', '南海出版公司',
        '温暖治愈的奇幻小说，讲述一家能解决烦恼的杂货店的故事。', '中文',
        '2014-05-01', 17),
       ('沉默的大多数', '王小波', '9787530215304', '北京十月文艺出版社',
        '王小波杂文集，展现其独特的思想和幽默的文风。', '中文', '1997-01-01', 9),
       ('哈利·波特与魔法石', 'J.K.罗琳', '9787020033430', '人民文学出版社',
        '魔法世界的第一部作品，讲述哈利·波特在霍格沃茨的第一年。', '中文',
        '2000-09-01', 35),
       ('乡土中国', '费孝通', '9787108018144', '生活·读书·新知三联书店',
        '中国社会学经典著作，分析中国传统乡土社会的结构特点。', '中文',
        '1948-01-01', 6),
       ('挪威的森林', '村上春树', '9787532751450', '上海译文出版社',
        '青春恋爱小说，讲述主人公渡边与两位女孩的爱情故事。', '中文',
        '2001-02-01', 14),
       ('设计心理学', '唐纳德·A·诺曼', '9787115356192', '中信出版社',
        '探讨日常物品设计中的心理学原理，影响设计思维的重要著作。', '中文',
        '2010-03-01', 11),
       ('经济学原理', '曼昆', '9787301250058', '北京大学出版社',
        '经济学入门经典教材，系统介绍经济学基本原理。', '中文', '2015-05-01', 20),
       ('万历十五年', '黄仁宇', '9787108009821', '生活·读书·新知三联书店',
        '以1587年为切入点，剖析明代中国社会的结构性矛盾。', '中文', '1982-05-01',
        7),
       ('月亮与六便士', '毛姆', '9787544258975', '南海出版公司',
        '以高更为原型，讲述一个证券经纪人放弃一切追求艺术的故事。', '中文',
        '2016-01-01', 13);