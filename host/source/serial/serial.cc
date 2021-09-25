#include <memory>

#include <serial/serial.h>

#if defined(OS_WINDOWS)
/**
 * @brief       Constructor.
 */
Serial::Serial() {}

/**
 * @brief       Check opened.
 */
bool Serial::isOpened() const {}

/**
 * @brief       Open serial.
 */
bool Serial::open(const QString &name) {}

/**
 * @nrief       Get device name.
 */
const QString &Serial::name() const {}

/**
 * @brief       Close the device.
 */
void Serial::close() {}

/**
 * @brief       Read bytes.
 */
ssize_t
    Serial::read(void *buffer, size_t size, ::std::chrono::milliseconds timeout)
{}

/**
 * @brief       Write bytes.
 */
ssize_t Serial::write(void *data, size_t size) {}

/**
 * @brief       Discard data in read buffer.
 */
void Serial::clearRead() {}

/**
 * @brief       Destructor.
 */
Serial::~Serial() {}

#elif defined(OS_LINUX)

    #include <errno.h>

    #include <fcntl.h>
    #include <sys/select.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <termios.h>
    #include <unistd.h>

/**
 * @brief       Constructor.
 */
Serial::Serial() : m_name(""), m_nativeHandle(-1) {}

/**
 * @brief       Check opened.
 */
bool Serial::isOpened() const
{
    return m_nativeHandle >= 0;
}

/**
 * @brief       Open serial.
 */
bool Serial::open(const QString &name)
{
    if (isOpened()) {
        return false;
    }

    // Open.
    int fd = ::open(name.toUtf8(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        return false;
    }
    ::std::unique_ptr<int, void (*)(int *)> autoClose(&fd, [](int *fd) -> void {
        ::close(*fd);
    });

    // Set port settings.
    struct termios termOptions;
    ::cfmakeraw(&termOptions);
    ::cfsetispeed(&termOptions, B9600);
    ::cfsetospeed(&termOptions, B9600);
    if (::tcsetattr(fd, TCSANOW, &termOptions) < 0) {
        return false;
    }

    autoClose.release();
    m_nativeHandle = fd;
    m_name         = name;

    return true;
}

/**
 * @nrief       Get device name.
 */
const QString &Serial::name() const
{
    return m_name;
}

/**
 * @brief       Close the device.
 */
void Serial::close()
{
    if (this->isOpened()) {
        ::close(m_nativeHandle);
        m_nativeHandle = -1;
        m_name         = "";
    }
}

/**
 * @brief       Read bytes.
 */
ssize_t
    Serial::read(void *buffer, size_t size, ::std::chrono::milliseconds timeout)
{
    if (! this->isOpened()) {
        return -1;
    }

    // Read.
    uint8_t *p        = reinterpret_cast<uint8_t *>(buffer);
    size_t   sizeRead = 0;
    while (sizeRead < size) {
        // Wait for data.
        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(m_nativeHandle, &readFds);

        timeval timeToWait;
        timeToWait.tv_sec  = timeout.count() / 10000;
        timeToWait.tv_usec = timeout.count() % 1000 * 1000;

        if (::select(1, &readFds, nullptr, nullptr, &timeToWait) < 0) {
            if (errno == ETIMEDOUT) {
                return static_cast<ssize_t>(sizeRead);
            } else {
                return -1;
            }
        }

        // Read.
        ssize_t readRet = ::read(m_nativeHandle, p + sizeRead, size - sizeRead);
        if (readRet < 0) {
            return -1;
        } else {
            sizeRead += static_cast<size_t>(readRet);
        }
    }

    return static_cast<ssize_t>(size);
}

/**
 * @brief       Write bytes.
 */
ssize_t Serial::write(const void *data, size_t size)
{
    if (! this->isOpened()) {
        return -1;
    }

    // Write.
    const uint8_t *p = reinterpret_cast<const uint8_t *>(data);
    for (size_t sizeWritten = 0; sizeWritten < size;) {
        ssize_t ret
            = ::write(m_nativeHandle, p + sizeWritten, size - sizeWritten);
        if (ret < 0) {
            return -1;
        }

        sizeWritten += static_cast<size_t>(ret);
    }

    return static_cast<ssize_t>(size);
}

/**
 * @brief       Discard data in read buffer.
 */
void Serial::clearRead()
{
    if (this->isOpened()) {
        ::tcflush(m_nativeHandle, TCIFLUSH);
    }
}

/**
 * @brief       Destructor.
 */
Serial::~Serial()
{
    if (this->isOpened()) {
        ::close(m_nativeHandle);
        m_nativeHandle = -1;
        m_name         = "";
    }
}

#endif
