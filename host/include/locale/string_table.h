#pragma once

#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>

/**
 * @brief   String table.
 */
class StringTable : public QObject {
    Q_OBJECT
  private:
    QReadWriteLock                        m_lock;        ///< Lock;
    QMap<QString, QMap<QString, QString>> m_strings;     ///< Strings.
    QString                               m_language;    ///< Language.
    uint32_t                              m_languageID;  ///< Language ID.
    QMap<QString, QMap<QString, QString>> m_stringTable; ///< String table.
    QString                               m_notFoundStr; ///< Default string.
    QMap<QString, QString>                m_notFoundMap; ///< Not found map.
  private:
    static QMap<int, QString>
        _languageTable; ///< Convert qt language to language string.
    static QMap<QString, QLocale>
        _qtLanguageTable; ///< Convert language string to qt language.
    static QMap<QString, uint32_t>
        _languageIDTable; ///< Convert language string to ID.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent      Parent object.
     */
    StringTable(QObject *parent);

    /**
     * @brief	Get current language.
     *
     * @return	Current language.
     */
    const QString &language();

    /**
     * @brief	Get current language ID.
     *
     * @return	Current language ID.
     */
    uint32_t languageId();

    /**
     * @brief		Get string.
     *
     * @param[in]	id		String ID.
     *
     * @return		String.
     */
    const QString &getString(const QString &id);

    /**
     * @brief		Get string in all languages.
     *
     * @param[in]	id		String ID.
     *
     * @return		Map of strings.
     */
    const QMap<QString, QString> &getStrings(const QString &id);

  public slots:
    /**
     * @brief       Set current language.
     *  Set current locale to locale. If the locale does not supported,
     *  current locale will be set to default locale.
     *
     * @param[in]   language		Language.
     *
     */
    void setLanguage(const QString &language);

  signals:
    /**
     * @brief   Emit when locale changes.
     */
    void languageChanged();

    /**
     * @brief   Emit after languageChanged().
     */
    void afterLanguageChanged();

  public:
    /**
     * @brief Destructor.
     */
    virtual ~StringTable();

  private:
    /**
     * @brief	Get system locale.
     *
     * @return	System locale.
     */
    QString systemLanguage();

    /**
     * @brief	Update default locale.
     */
    void updateLocale();
};
