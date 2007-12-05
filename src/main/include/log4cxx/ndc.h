/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOG4CXX_NDC_H
#define _LOG4CXX_NDC_H

#include <log4cxx/log4cxx.h>
#include <log4cxx/logstring.h>
#include <stack>

namespace log4cxx
{
        /**
        the ndc class implements <i>nested diagnostic contexts</i> as
        defined by neil harrison in the article "patterns for logging
        diagnostic messages" part of the book "<i>pattern languages of
        program design 3</i>" edited by martin et al.

        <p>a nested diagnostic context, or ndc in short, is an instrument
        to distinguish interleaved log output from different sources. log
        output is typically interleaved when a server handles multiple
        clients near-simultaneously.

        <p>interleaved log output can still be meaningful if each log entry
        from different contexts had a distinctive stamp. this is where ndcs
        come into play.

        <p><em><b>note that ndcs are managed on a per thread
        basis</b></em>. ndc operations such as #push,
        #pop, #clear and #getDepth
        affect the ndc of the <em>current</em> thread only. ndcs of other
        threads remain unaffected.

        <p>for example, a servlet can build a per client request ndc
        consisting the clients host name and other information contained in
        the the request. <em>cookies</em> are another source of distinctive
        information. to build an ndc one uses the #push
        operation. simply put,

        <p><ul>
         <li>contexts can be nested.

         <p><li>when entering a context, call <code>ndc.push</code>. as a
         side effect, if there is no nested diagnostic context for the
         current thread, this method will create it.

         <p><li>when leaving a context, call <code>ndc.pop</code>.

         <p><li><b>when exiting a thread make sure to call #remove
         </b>.
        </ul>

        <p>there is no penalty for forgetting to match each
        <code>push</code> operation with a corresponding <code>pop</code>,
        except the obvious mismatch between the real application context
        and the context set in the ndc.

        <p>if configured to do so, PatternLayout and
        TTCCLayout instances automatically retrieve the nested diagnostic
        context for the current thread without any user intervention.
        hence, even if a servlet is serving multiple clients
        simultaneously, the logs emanating from the same code (belonging to
        the same logger) can still be distinguished because each client
        request will have a different ndc tag.

        <p>heavy duty systems should call the #remove method when
        leaving the run method of a thread. this ensures that the memory
        used by the thread can be freed by the java garbage
        collector. there is a mechanism to lazily remove references to dead
        threads. in practice, this means that you can be a little sloppy
        and sometimes forget to call #remove before exiting a
        thread.

        <p>a thread may inherit the nested diagnostic context of another
        (possibly parent) thread using the #inherit
        method. a thread may obtain a copy of its ndc with the
        #clonestack method and pass the reference to any other
        thread, in particular to a child.
        */
        class LOG4CXX_EXPORT NDC
        {
        private:
                class DiagnosticContext
                {
                public:
                        LogString fullMessage;
                        LogString message;

                        DiagnosticContext(const LogString& message,
                                const DiagnosticContext * parent);
                        virtual ~DiagnosticContext();
                        DiagnosticContext(const DiagnosticContext& src);
                        DiagnosticContext& operator=(const DiagnosticContext& src);
                };

        public:
                typedef std::stack<DiagnosticContext> Stack;

                /**
                 Creates a nested diagnostic context.
                 Since java performs no automatic cleanup of objects when a
                 scope is left, in log4j push() and pop() must be used
                 to manage the NDC. For convenience, log4cxx provides 
                 an NDC constructor and destructor which simply call the push() and
                 pop() methods, allowing for automatic cleanup when the current
                 scope ends.

                 @param message The new diagnostic context information.
                 @see The #push method.
                 */
#if LOG4CXX_HAS_WCHAR_T
                NDC(const std::wstring& message);
#endif
                NDC(const std::string& message);

                /**
                Removes the topmost element from the NDC stack.

                @see The #pop method.
                */
                ~NDC();

                /**
                Clear any nested diagnostic information if any. This method is
                useful in cases where the same thread can be potentially used
                over and over in different unrelated contexts.
                <p>This method is equivalent to calling the #setMaxDepth
                method with a zero <code>maxDepth</code> argument.
                */
                static void clear();


                /**
                <b>Never use this method directly, use the
                {@link spi::LoggingEvent#getNDC LoggingEvent::getNDC}
                method instead.</b>
                * @param dest destination to which to append content of NDC.
                * @return true if NDC is set.
                */
                static bool get(LogString& dest);

                /**
                Get the current nesting depth of this diagnostic context.
                */
                static int getDepth();


                /**
                * Tests if the NDC is empty.
                */
                static bool empty();

                /**
                Clients should call this method before leaving a diagnostic
                context.
                <p>The returned value is the value that was pushed last. If no
                context is available, then the empty string "" is returned.
                @return The innermost diagnostic context.
                */
                static LogString pop();

                /**
                Looks at the last diagnostic context at the top of this NDC
                without removing it.
                <p>The returned value is the value that was pushed last. If no
                context is available, then the empty string "" is returned.
                @return String The innermost diagnostic context.
                */
                static LogString peek();

                /**
                Push new diagnostic context information for the current thread.
                <p>The contents of the <code>message</code> parameter is
                determined solely by the client.
                @param message The new diagnostic context information.
                */
#if LOG4CXX_HAS_WCHAR_T
                static void push(const std::wstring& message);
#endif
                static void push(const std::string& message);
                static void pushLogString(const LogString& message);

                /**
                Remove the diagnostic context for this thread.
                <p>Each thread that created a diagnostic context by calling
                #push should call this method before exiting. Otherwise,
                the memory used by the <b>thread</b> cannot be reclaimed by the
                VM.
                <p>As this is such an important problem in heavy duty systems and
                because it is difficult to always guarantee that the remove
                method is called before exiting a thread, this method has been
                augmented to lazily remove references to dead threads. In
                practice, this means that you can be a little sloppy and
                occasionally forget to call #remove before exiting a
                thread. However, you must call <code>remove</code> sometime. If
                you never call it, then your application is sure to run out of
                memory.
                */
                static void remove();
        private:
                NDC(const NDC&);
                NDC& operator=(const NDC&);
        }; // class NDC;
}  // namespace log4cxx

#endif // _LOG4CXX_NDC_H
