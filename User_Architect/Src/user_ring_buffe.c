/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_ring_buffe.h"

/* 私有函数声明 --------------------------------------------------------------*/
static void RingBuffer_AddReadIndex(RING_BUFFER *buffer, uint16_t length);
static uint8_t RingBuffer_Read(const RING_BUFFER *buffer, uint16_t index);
static uint16_t RingBuffer_GetRemain(const RING_BUFFER *buffer);

/* 私有函数 ------------------------------------------------------------------*/

/**
* @brief 增加读索引
* @param buffer 环形缓冲区指针
* @param length 要增加的长度
*/
static void RingBuffer_AddReadIndex(RING_BUFFER *buffer, uint16_t length) {
    buffer->read_index += length;
    buffer->read_index %= RING_BUFFER_SIZE;
}

/**
* @brief 读取第 i 位数据（超过缓冲区长度自动循环）
* @param buffer 环形缓冲区指针
* @param index  要读取的数据索引
* @return 读取的数据
*/
static uint8_t RingBuffer_Read(const RING_BUFFER *buffer, uint16_t index) {
    return buffer->buffer[index % RING_BUFFER_SIZE];
}

/**
* @brief 计算缓冲区剩余空间
* @param buffer 环形缓冲区指针
* @return 剩余空间大小
*/
static uint16_t RingBuffer_GetRemain(const RING_BUFFER *buffer) {
    return RING_BUFFER_SIZE - RingBuffer_GetLength(buffer);
}

/* 函数体 --------------------------------------------------------------------*/

/**
* @brief 计算未处理的数据长度
* @param buffer 环形缓冲区指针
* @return 未处理的数据长度
*/
uint16_t RingBuffer_GetLength(const RING_BUFFER *buffer) {
    return (buffer->write_index + RING_BUFFER_SIZE - buffer->read_index) % RING_BUFFER_SIZE;
}


/**
* @brief 向缓冲区写入数据
* @param buffer 环形缓冲区指针
* @param data   要写入的数据指针
* @param length 要写入的数据长度
* @return 写入的数据长度
* @retval 0 缓冲区空间不足，未写入
* @retval length 成功写入的数据长度
*/
uint16_t RingBuffer_Put(RING_BUFFER *buffer, const uint8_t *data, uint16_t length) {
    // 如果缓冲区空间不足，则不写入数据
    if (RingBuffer_GetRemain(buffer) < length) {
        return 0;
    }
    
    // 将数据写入缓冲区
    if (buffer->write_index + length < RING_BUFFER_SIZE) {
        memcpy(buffer->buffer + buffer->write_index, data, length);
        buffer->write_index += length;
    } else {
        const uint16_t first_length = RING_BUFFER_SIZE - buffer->write_index;
        memcpy(buffer->buffer + buffer->write_index, data, first_length);
        memcpy(buffer->buffer, data + first_length, length - first_length);
        buffer->write_index = length - first_length;
    }
    return length;
}

/**
* @brief 获取一段带有帧头和帧尾的数据帧
* @param buffer  环形缓冲区指针
* @param message 存储获取数据的缓冲区
* @param head    帧头字符串
* @param tail    帧尾字符串
* @return 获取的数据帧长度
* @retval 0 没有获取到数据帧
* @retval >0 获取到的数据帧长度
*/
uint16_t RingBuffer_GetWithHT(RING_BUFFER *buffer, uint8_t *message, const char *head, const char *tail) {
    const uint16_t buff_len = RingBuffer_GetLength(buffer);
    const uint16_t head_len = (uint16_t)strlen(head);
    const uint16_t tail_len = (uint16_t)strlen(tail);
    
    if (buff_len >= head_len + tail_len) {
        for (int32_t start_i = (int32_t)buff_len - (int32_t)head_len - (int32_t)tail_len; start_i >= 0; start_i--) {
            uint8_t head_match = 1;
            for (uint16_t x = 0; x < head_len; x++) {
                head_match &= RingBuffer_Read(buffer, buffer->read_index + start_i + x) == head[x];
            }
            
            if (head_match) {
                for (uint16_t tail_i = head_len; tail_i <= buff_len - tail_len - start_i; tail_i++) {
                    uint8_t tail_match = 1;
                    for (uint16_t x = 0; x < tail_len; x++) {
                        tail_match &= RingBuffer_Read(buffer, buffer->read_index + start_i + tail_i + x) == tail[x];
                    }
                    
                    if (tail_match) {
                        const uint16_t frame_len = tail_i + tail_len;
                        if (buffer->read_index + start_i + frame_len >= RING_BUFFER_SIZE) {
                            const uint16_t first_part = RING_BUFFER_SIZE - (buffer->read_index + start_i);
                            memcpy(message, buffer->buffer + buffer->read_index + start_i, first_part);
                            memcpy(message + first_part, buffer->buffer, frame_len - first_part);
                        } else {
                            memcpy(message, buffer->buffer + buffer->read_index + start_i, frame_len);
                        }
                        RingBuffer_AddReadIndex(buffer, start_i + frame_len);
                        return frame_len;
                    }
                }
            }
        }
    }
    return 0;
}

/**
* @brief 通过数据帧的长度和帧头获取数据帧
* @param buffer  环形缓冲区指针
* @param message 存储获取数据的缓冲区
* @param head    帧头字符串
* @param len     数据帧长度
* @return 获取的数据帧长度
* @retval 0 没有获取到数据帧
* @retval len 成功获取到数据帧
*/
uint16_t RingBuffer_GetWithHLen(RING_BUFFER *buffer, uint8_t *message, const char *head, uint16_t len) {
    const uint16_t buff_len = RingBuffer_GetLength(buffer);
    const uint16_t head_len = (uint16_t)strlen(head);
    
    if (buff_len >= len) {
        for (int32_t start_i = (int32_t)buff_len - len; start_i >= 0; start_i--) {
            uint8_t head_match = 1;
            for (uint16_t x = 0; x < head_len; x++) {
                head_match &= RingBuffer_Read(buffer, buffer->read_index + start_i + x) == head[x];
            }
            
            if (head_match) {
                if (buffer->read_index + start_i + len >= RING_BUFFER_SIZE) {
                    const uint16_t first_part = RING_BUFFER_SIZE - (buffer->read_index + start_i);
                    memcpy(message, buffer->buffer + buffer->read_index + start_i, first_part);
                    memcpy(message + first_part, buffer->buffer, len - first_part);
                } else {
                    memcpy(message, buffer->buffer + buffer->read_index + start_i, len);
                }
                RingBuffer_AddReadIndex(buffer, start_i + len);
                return len;
            }
        }
    }
    return 0;
}


/**
* @brief 通过数据帧的长度获取数据帧
* @param buffer  环形缓冲区指针
* @param message 存储获取数据的缓冲区
* @param len     数据帧长度
* @return 获取的数据帧长度
* @retval 0 缓冲区数据长度不足
* @retval len 成功获取到数据帧
*/
uint16_t RingBuffer_GetWithLen(RING_BUFFER *buffer, uint8_t *message, uint16_t len) {
    const uint16_t buff_len = RingBuffer_GetLength(buffer);
    
    if (buff_len >= len) {
        if (buffer->read_index + len >= RING_BUFFER_SIZE) {
            const uint16_t first_part = RING_BUFFER_SIZE - buffer->read_index;
            memcpy(message, buffer->buffer + buffer->read_index, first_part);
            memcpy(message + first_part, buffer->buffer, len - first_part);
        } else {
            memcpy(message, buffer->buffer + buffer->read_index, len);
        }
        RingBuffer_AddReadIndex(buffer, len);
        return len;
    }
    return 0;
}

/**
* @brief 获取从帧头到下一个帧头的数据帧
* @param buffer  环形缓冲区指针
* @param message 存储获取数据的缓冲区
* @param head    帧头字符串
* @return 获取的数据帧长度
* @retval 0 没有获取到数据帧
* @retval >0 获取到的数据帧长度
*/
uint16_t RingBuffer_GetWithH(RING_BUFFER *buffer, uint8_t *message, const char *head) {
    const uint16_t buff_len = RingBuffer_GetLength(buffer);
    const uint16_t head_len = (uint16_t)strlen(head);
    
    if (buff_len >= 2 * head_len) {
        for (int32_t start_i = 0; start_i <= (int32_t)buff_len - (int32_t)head_len; start_i++) {
            uint8_t head_match = 1;
            for (uint16_t x = 0; x < head_len; x++) {
                head_match &= RingBuffer_Read(buffer, buffer->read_index + start_i + x) == head[x];
            }
            
            if (head_match) {
                for (uint16_t tail_i = head_len; tail_i <= buff_len - head_len - start_i; tail_i++) {
                    uint8_t next_head_match = 1;
                    for (uint16_t x = 0; x < head_len; x++) {
                        next_head_match &= RingBuffer_Read(buffer, buffer->read_index + start_i + tail_i + x) == head[x];
                    }
                    
                    if (next_head_match) {
                        if (buffer->read_index + start_i + tail_i >= RING_BUFFER_SIZE) {
                            const uint16_t first_part = RING_BUFFER_SIZE - (buffer->read_index + start_i);
                            memcpy(message, buffer->buffer + buffer->read_index + start_i, first_part);
                            memcpy(message + first_part, buffer->buffer, tail_i - first_part);
                        } else {
                            memcpy(message, buffer->buffer + buffer->read_index + start_i, tail_i);
                        }
                        RingBuffer_AddReadIndex(buffer, start_i + tail_i);
                        return tail_i;
                    }
                }
            }
        }
    }
    return 0;
}