/*
 * Copyright 1999,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOG4CXX_PATTERN_MESSAGE_PATTERN_CONVERTER
#define _LOG4CXX_PATTERN_MESSAGE_PATTERN_CONVERTER

#include <log4cxx/pattern/loggingeventpatternconverter.h>

namespace log4cxx { namespace pattern {


/**
 * Formats the message of an logging event.
 *
 * @author Curt Arnold
 * @since 0.9.8
 */
class LOG4CXX_EXPORT MessagePatternConverter : public LoggingEventPatternConverter {

  /**
   * Private constructor.
   */
  MessagePatternConverter();

public:
DECLARE_LOG4CXX_PATTERN(MessagePatternConverter)
BEGIN_LOG4CXX_CAST_MAP()
     LOG4CXX_CAST_ENTRY(MessagePatternConverter)
     LOG4CXX_CAST_ENTRY_CHAIN(LoggingEventPatternConverter)
END_LOG4CXX_CAST_MAP()

  /**
   * Obtains an instance of pattern converter.
   * @param options options, may be null.
   * @param logger  logger, current ignored, may be null.
   * @return instance of pattern converter.
   */
  static PatternConverterPtr newInstance(
    const std::vector<LogString>& options);

  /**
   * {@inheritDoc}
   */
  void format(const log4cxx::spi::LoggingEventPtr& event,
      LogString& toAppendTo,
      log4cxx::helpers::Pool& p) const;
};
}
}
#endif
