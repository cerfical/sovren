#pragma once

namespace reni {

    /**
     * @brief Designates the severity level of a log.
     */
    enum class LogKind {
        None,    /**< Empty default value. */
        Debug,   /**< Lowest priority messages: intended for debugging purposes only. */
        Info,    /**< Informational messages: could be useful to see how things work. **/
        Warning, /**< Warnings: to notify of possible problems. */
        Error    /**< For reporting internal exceptional situations. */
    };

}
