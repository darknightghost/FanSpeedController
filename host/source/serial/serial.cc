#include <memory>

#include <QtCore/QDebug>

#include <serial/serial.h>

#if defined(OS_WINDOWS)

    #include <Windows.h>

/**
 * @brief       Constructor.
 */
Serial::Serial() : m_name(""), m_nativeHandle(INVALID_HANDLE_VALUE) {}

/**
 * @brief       Check opened.
 */
bool Serial::isOpened() const
{
    return m_nativeHandle != INVALID_HANDLE_VALUE;
}

/**
 * @brief       Open serial.
 */
bool Serial::open(const QString &name)
{
    if (isOpened()) {
        return false;
    }

    // Open serial.
    HANDLE hnd = ::CreateFileW(name.toStdWString().c_str(),
                               GENERIC_READ | GENERIC_WRITE, 0, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hnd == INVALID_HANDLE_VALUE) {
        return false;
    }
    ::std::unique_ptr<HANDLE, void (*)(HANDLE *)> autoClose(
        &hnd, [](HANDLE *hnd) -> void {
            ::CloseHandle(*hnd);
        });

    // Set serial attribute.
    DCB dcb;
    if (! ::GetCommState(hnd, &dcb)) {
        return false;
    }
    dcb.BaudRate          = CBR_9600;
    dcb.fBinary           = TRUE;
    dcb.fParity           = FALSE;
    dcb.fOutxCtsFlow      = FALSE;
    dcb.fOutxDsrFlow      = FALSE;
    dcb.fDtrControl       = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity   = FALSE;
    dcb.fTXContinueOnXoff = FALSE;
    dcb.fOutX             = FALSE;
    dcb.fInX              = FALSE;
    dcb.fErrorChar        = FALSE;
    dcb.fRtsControl       = RTS_CONTROL_DISABLE;
    dcb.fAbortOnError     = FALSE;
    dcb.ByteSize          = 8;
    dcb.Parity            = NOPARITY;
    dcb.StopBits          = ONESTOPBIT;

    if (! ::SetCommState(hnd, &dcb)) {
        return false;
    }

    autoClose.release();
    m_nativeHandle = hnd;
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
        ::CloseHandle(m_nativeHandle);
        m_nativeHandle = INVALID_HANDLE_VALUE;
        m_name         = "";
    }
}

/**
 * @brief       Read bytes.
 */
ssize_t
    Serial::read(void *buffer, size_t size, ::std::chrono::milliseconds timeout)
{
    if (! isOpened()) {
        return -1;
    }

    // Set timeout.
    COMMTIMEOUTS commTimeouts;
    if (! ::GetCommTimeouts(m_nativeHandle, &commTimeouts)) {
        return -1;
    }
    commTimeouts.ReadIntervalTimeout = timeout.count();
    if (! ::SetCommTimeouts(m_nativeHandle, &commTimeouts)) {
        return -1;
    }

    // Read.
    uint8_t *p          = reinterpret_cast<uint8_t *>(buffer);
    DWORD    sizeToRead = static_cast<DWORD>(size);
    DWORD    sizeRead   = 0;
    while (sizeRead < sizeToRead) {
        DWORD sz = 0;
        if (! ::ReadFile(m_nativeHandle, p + sizeRead, sizeToRead - sizeRead,
                         &sz, nullptr)) {
            return -1;
        }
        if (sz == 0) {
            return static_cast<ssize_t>(sizeRead);
            ;
        } else {
            sizeRead += sz;
        }
    }

    return static_cast<ssize_t>(sizeRead);
}

/**
 * @brief       Write bytes.
 */
ssize_t Serial::write(const void *data, size_t size)
{
    if (! isOpened()) {
        return -1;
    }

    DWORD          sizeWritten = 0;
    const uint8_t *p           = reinterpret_cast<const uint8_t *>(data);
    DWORD          sizeToWrite = static_cast<DWORD>(size);
    while (sizeWritten < sizeToWrite) {
        DWORD written = 0;
        if (! ::WriteFile(m_nativeHandle, p + sizeWritten,
                          sizeToWrite - sizeWritten, &written, nullptr)) {
            return -1;
        }

        sizeWritten += written;
    }

    return 0;
}

/**
 * @brief       Discard data in read buffer.
 */
void Serial::clearRead()
{
    if (this->isOpened()) {
        DWORD   errors;
        COMSTAT stat;
        ::ClearCommError(m_nativeHandle, &errors, &stat);
        ::PurgeComm(m_nativeHandle, PURGE_RXCLEAR);
    }
}

/**
 * @brief       Destructor.
 */
Serial::~Serial()
{
    if (this->isOpened()) {
        ::CloseHandle(m_nativeHandle);
        m_nativeHandle = INVALID_HANDLE_VALUE;
        m_name         = "";
    }
}

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
    int fd = ::open(QString("/dev/%1").arg(name).toUtf8(), O_RDWR | O_NOCTTY);
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
    termOptions.c_cc[VTIME] = 0;
    termOptions.c_cc[VMIN]  = 1;
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
            char errBuf[256];
            int  err = errno;
            qWarning() << "Select failed with errno " << err << " : "
                       << ::strerror_r(err, errBuf, sizeof(errBuf)) << ".";
            if (errno == ETIMEDOUT) {
                return static_cast<ssize_t>(sizeRead);
            } else {
                return -1;
            }
        }

        // Read.
        ssize_t readRet = ::read(m_nativeHandle, p + sizeRead, size - sizeRead);
        if (readRet < 0) {
            char errBuf[256];
            int  err = errno;
            qWarning() << "Read failed with errno " << err << " : "
                       << ::strerror_r(err, errBuf, sizeof(errBuf)) << ".";
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
            char errBuf[256];
            int  err = errno;
            qWarning() << "Write failed with errno " << err << " : "
                       << ::strerror_r(err, errBuf, sizeof(errBuf)) << ".";
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
