#pragma once

#include <chrono>
#include <string>
#include <cstdint>

#include <QtCore/QString>

#if defined(OS_WINDOWS)
    #include <Windows.h>
    
    typedef     int64_t     ssize_t;

#elif defined(OS_LINUX)
    #include <unistd.h>

#endif

/**
 * @brief       Serial wrapper.
 */
class Serial {
  public:
#if defined(OS_WINDOWS)
    using NativeHandle = HANDLE;

#elif defined(OS_LINUX)
    using NativeHandle = int;

#endif
  private:
    QString      m_name;         ///< Device name.
    NativeHandle m_nativeHandle; ///< Native handle.

  public:
    /**
     * @brief       Constructor.
     */
    Serial();
    Serial(const Serial &) = delete;
    Serial(Serial &&)      = delete;

    /**
     * @brief       Check opened.
     *
     * @return      \c true if opened, otherwise returns false.
     */
    bool isOpened() const;

    /**
     * @brief       Open serial.
     *
     * @param[in]   name        Device name.
     *
     * @return      \c true if succcess, otherwise returns false.
     */
    bool open(const QString &name);

    /**
     * @nrief       Get device name.
     *
     * @return      Device name.
     */
    const QString &name() const;

    /**
     * @brief       Close the device.
     */
    void close();

    /**
     * @brief       Read bytes.
     *
     * @param[out]  buffer      Output buffer.
     * @param[in]   size        Size to read.
     * @param[in]   timeout     Timeout.
     *
     * @return      On success or time outed, the numbner of bytes read
     *              returned, otherwise returns -1.
     */
    ssize_t
        read(void *buffer, size_t size, ::std::chrono::milliseconds timeout);

    /**
     * @brief       Write bytes.
     *
     * @param[in]   data        Data to write.
     * @param[in]   size        Size to write
     *
     * @return      On success, the numbner of bytes written returned, otherwise
     *              returns -1.
     */
    ssize_t write(const void *data, size_t size);

    /**
     * @brief       Discard data in read buffer.
     */
    void clearRead();

    /**
     * @brief       Destructor.
     */
    virtual ~Serial();
};
