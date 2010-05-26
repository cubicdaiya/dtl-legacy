/**
   dtl-1.06 -- Diff Template Library
   
   In short, Diff Template Library is distributed under so called "BSD license",
   
   Copyright (c) 2008-2010 Tatsuhiko Kubo <cubicdaiya@gmail.com>
   All rights reserved.
   
   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:
   
   * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
   
   * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
   
   * Neither the name of the authors nor the names of its contributors
   may be used to endorse or promote products derived from this software 
   without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* If you use this library, you must include dtl.hpp only. */

#ifndef DTL_DIFF3_H
#define DTL_DIFF3_H

namespace dtl {
    
    /**
     * diff3 class template
     * sequence must support random_access_iterator.
     */
    template <typename elem, typename sequence>
    class Diff3
    {
        typedef pair< elem, elemInfo > sesElem;
        typedef vector< sesElem > sesElemVec;
        typedef vector< elem > elemVec;
        typedef typename sesElemVec::iterator sesElemVec_iter;
        typedef typename sequence::iterator sequence_iter;
        typedef typename elemVec::iterator elemVec_iter;
    private:
        sequence A;
        sequence B;
        sequence C;
        sequence S;
        Diff< elem, sequence > diff_ba;
        Diff< elem, sequence > diff_bc;
        bool conflict;
        elem csepabegin;
        elem csepa;
        elem csepaend;
    public :
        Diff3 () {}
        Diff3 (const sequence& a, 
               const sequence& b, 
               const sequence& c) : A(a), B(b), C(c), 
                                    diff_ba(b, a), diff_bc(b, c), 
                                    conflict(false) {} 
        
        ~Diff3 () {}
        
        bool isConflict () const {
            return conflict;
        }
        
        void setConflictSeparators (const elem begin, const elem sepa,const elem end) {
            csepabegin = begin;
            csepa      = sepa;
            csepaend   = end;
        }
        
        sequence getMergedSequence () const {
            return S;
        }
        
        /**
         * merge changes B and C to A
         */
        bool merge () {
            if (diff_ba.getEditDistance() == 0) {     // A == B
                if (diff_bc.getEditDistance() == 0) { // A == B == C
                    S = B;
                    return true;
                }
                S = C;
                return true;
            } else {                                  // A != B
                if (diff_bc.getEditDistance() == 0) { // A != B == C
                    S = A;                              
                    return true;
                } else {                              // A != B != C
                    S = merge_();
                    if (isConflict()) {               // conflict occured
                        specifyConfliction();
                        return false;
                    }
                }
            }
            return true;
        }
        
        /**
         * compose differences
         */
        void compose () {
            diff_ba.compose();
            diff_bc.compose();
        }
        
    private :
        /**
         * merge implementation
         */
        sequence merge_ () {
            elemVec seq;
            Ses< elem > ses_ba     = diff_ba.getSes();
            Ses< elem > ses_bc     = diff_bc.getSes();
            sesElemVec ses_ba_v    = ses_ba.getSequence();
            sesElemVec ses_bc_v    = ses_bc.getSequence();
            sesElemVec_iter ba_it  = ses_ba_v.begin();
            sesElemVec_iter bc_it  = ses_bc_v.begin();
            sesElemVec_iter ba_end = ses_ba_v.end();
            sesElemVec_iter bc_end = ses_bc_v.end();
            
            while (!isEnd(ba_end, ba_it) || !isEnd(bc_end, bc_it)) {
                while (true) {
                    if (!isEnd(ba_end, ba_it)            && 
                        !isEnd(bc_end, bc_it)            &&
                        ba_it->first == bc_it->first     && 
                        ba_it->second.type == SES_COMMON && 
                        bc_it->second.type == SES_COMMON) {
                        // do nothing
                    } else {
                        break;
                    }
                    if      (!isEnd(ba_end, ba_it)) seq.push_back(ba_it->first);
                    else if (!isEnd(bc_end, bc_it)) seq.push_back(bc_it->first);
                    forwardUntilEnd(ba_end, ba_it);
                    forwardUntilEnd(bc_end, bc_it);
                }
                if (isEnd(ba_end, ba_it) || isEnd(bc_end, bc_it)) break;
                if (   ba_it->second.type == SES_COMMON 
                       && bc_it->second.type == SES_DELETE) {
                    forwardUntilEnd(ba_end, ba_it);
                    forwardUntilEnd(bc_end, bc_it);
                } else if (ba_it->second.type == SES_COMMON && 
                           bc_it->second.type == SES_ADD) {
                    seq.push_back(bc_it->first);
                    forwardUntilEnd(bc_end, bc_it);
                } else if (ba_it->second.type == SES_DELETE && 
                           bc_it->second.type == SES_COMMON) {
                    forwardUntilEnd(ba_end, ba_it);
                    forwardUntilEnd(bc_end, bc_it);
                } else if (ba_it->second.type == SES_DELETE && 
                           bc_it->second.type == SES_DELETE) {
                    if (ba_it->first == bc_it->first) {
                        forwardUntilEnd(ba_end, ba_it);
                        forwardUntilEnd(bc_end, bc_it);
                    } else {
                        // conflict
                        conflict = true;
                        return B;
                    }
                } else if (ba_it->second.type == SES_DELETE && 
                           bc_it->second.type == SES_ADD) {
                    // conflict
                    conflict = true;
                    return B;
                } else if (ba_it->second.type == SES_ADD && 
                           bc_it->second.type == SES_COMMON) {
                    seq.push_back(ba_it->first);
                    forwardUntilEnd(ba_end, ba_it);
                } else if (ba_it->second.type == SES_ADD && 
                           bc_it->second.type == SES_DELETE) {
                    // conflict
                    conflict = true;
                    return B;
                } else if (ba_it->second.type == SES_ADD && 
                           bc_it->second.type == SES_ADD) {
                    if (ba_it->first == bc_it->first) {
                        seq.push_back(ba_it->first);
                        forwardUntilEnd(ba_end, ba_it);
                        forwardUntilEnd(bc_end, bc_it);
                    } else {
                        // conflict
                        conflict = true;
                        return B;
                    }
                }
            }
            
            if (isEnd(ba_end, ba_it)) {
                addDecentSequence(bc_end, bc_it, seq);
            } else if (isEnd(bc_end, bc_it)) {
                addDecentSequence(ba_end, ba_it, seq);
            }
            
            sequence mergedSeq(seq.begin(), seq.end());
            return mergedSeq;
        }
        
        /**
         * specify the confliction points
         */
        void specifyConfliction () {
            sequence_iter a_it         = A.begin();
            sequence_iter b_it         = B.begin();
            sequence_iter c_it         = C.begin();
            sequence_iter a_end        = A.end();
            sequence_iter b_end        = B.end();
            sequence_iter c_end        = C.end();
            elemVec elem_common;
            elemVec elem_conf1;
            elemVec elem_conf2;
            elemVec seq_vec;
            bool is_common;
            elemVec elem_common_end_vec;
            
            is_common = false;
        SPECIFY :
            elem_common.clear();
            elem_common_end_vec.clear();
            elem_conf1.clear();
            elem_conf2.clear();
            while (!isEnd(a_end, a_it) ||
                   !isEnd(b_end, b_it) ||
                   !isEnd(c_end, c_it)) {
                if (*b_it == *a_it && *b_it == *c_it) {
                    if (!is_common && b_it != B.begin()) {
                        if (a_it != A.end()) elem_common_end_vec.push_back(*a_it);
                        forwardUntilEnd(a_end, a_it);
                        forwardUntilEnd(b_end, b_it);
                        forwardUntilEnd(c_end, c_it);
                        break;
                    }
                    if (a_it != A.end()) elem_common.push_back(*a_it);
                    is_common = true;
                } else if (*b_it == *a_it && *b_it != *c_it) {
                    if (is_common) {
                        elem_conf1.push_back(csepabegin);
                        elem_conf2.push_back(csepa);
                    }
                    if (b_it != B.end()) elem_conf1.push_back(*b_it);
                    if (c_it != C.end()) elem_conf2.push_back(*c_it);
                    is_common = false;
                } else if (*b_it == *c_it && *b_it != *a_it) {
                    if (is_common) {
                        elem_conf1.push_back(csepabegin);
                        elem_conf2.push_back(csepa);
                    }
                    if (a_it != A.end()) elem_conf1.push_back(*a_it);
                    if (c_it != C.end()) elem_conf2.push_back(*c_it);
                    is_common = false;
                    if (a_it == a_end && c_it == c_end) break;
                } else if (*b_it != *a_it && *b_it != *c_it) {
                    if (is_common || b_it == B.begin()) {
                        elem_conf1.push_back(csepabegin);
                        elem_conf2.push_back(csepa);
                    }
                    if (a_it != A.end()) elem_conf1.push_back(*a_it);
                    if (c_it != C.end()) elem_conf2.push_back(*c_it);
                    is_common = false;
                    if (a_it == a_end && c_it == c_end) break;
                }
                forwardUntilEnd(a_end, a_it);
                forwardUntilEnd(b_end, b_it);
                forwardUntilEnd(c_end, c_it);
            }
            
            joinElemVec(seq_vec, elem_common);
            if (elem_conf1[0] == csepabegin && elem_conf1.size() == 1) {
                elem_conf1.clear();
                elem_conf2 = elemVec(elem_conf2.begin() + 1, elem_conf2.end());
            }
            joinElemVec(seq_vec, elem_conf1);
            joinElemVec(seq_vec, elem_conf2);

            if ((!elem_conf1.empty() && !elem_conf2.empty())) {
                seq_vec.push_back(csepaend);
            }

            joinElemVec(seq_vec, elem_common_end_vec);
            
            if (isEnd(a_end, a_it) &&
                isEnd(b_end, b_it) &&
                isEnd(c_end, c_it)) {
                // do nothing
            } else if (!isEnd(a_end, a_it) &&
                       isEnd(b_end, b_it) &&
                       isEnd(c_end, c_it)) {
                addSpecifiedSequence(a_end, a_it, seq_vec, b_it, b_end, false);
                is_common = true;
                goto SPECIFY;
            } else if (isEnd(a_end,  a_it) &&
                       !isEnd(b_end, b_it) &&
                       isEnd(c_end,  c_it)) {
                addSpecifiedSequence(b_end, b_it, seq_vec, b_it, b_end, false);
            } else if (isEnd(a_end, a_it) &&
                       isEnd(b_end, b_it) &&
                       !isEnd(c_end, c_it)) {
                addSpecifiedSequence(c_end, c_it, seq_vec, b_it, b_end, false);
            } else if (isEnd(a_end,  a_it) &&
                       !isEnd(b_end, b_it) &&
                       !isEnd(c_end, c_it)) {
                addSpecifiedSequence(c_end, c_it, seq_vec, b_it, b_end, true);
                addSpecifiedSequence(b_end, b_it, seq_vec, b_it, b_end, false);
            } else if (!isEnd(a_end, a_it) &&
                       isEnd(b_end, b_it) &&
                       !isEnd(c_end, c_it)) {
                is_common = true;
                goto SPECIFY;
            } else if (!isEnd(a_end, a_it) &&
                       !isEnd(b_end, b_it) &&
                       isEnd(c_end, c_it)) {
                addSpecifiedSequence(a_end, a_it, seq_vec, b_it, b_end, true);
                addSpecifiedSequence(b_end, b_it, seq_vec, b_it, b_end, false);
            } else {
                is_common = true;
                goto SPECIFY;
            }
            
            sequence mergedSeq(seq_vec.begin(), seq_vec.end());
            S = mergedSeq;
        }
        
        /**
         * join elem vectors
         */
        void inline joinElemVec (elemVec& s1, elemVec& s2) const {
            if (!s2.empty()) {
                for (elemVec_iter vit=s2.begin();vit!=s2.end();++vit) {
                    s1.push_back(*vit);
                }
            }
        }
        
        /**
         * check sequence is end
         */
        template <typename T_iter>
        bool inline isEnd (T_iter& end, T_iter& it) const {
            return it == end ? true : false;
        }
        
        /**
         * increment iterator until iterator is end
         */
        template <typename T_iter>
        void inline forwardUntilEnd (T_iter& end, T_iter& it) const {
            if (!isEnd(end, it)) ++it;
        }
        
        /**
         * add elements which SES's type is ADD
         */
        void inline addDecentSequence (sesElemVec_iter& end, sesElemVec_iter& it, elemVec& seq) const {
            while (!isEnd(end, it)) {
                if (it->second.type == SES_ADD) seq.push_back(it->first);
                ++it;
            }      
        }
        
        /**
         * add confliction specified sequence
         */
        void inline addSpecifiedSequence(sequence_iter& end, sequence_iter& it, elemVec& seq, 
                                         sequence_iter& b_it, sequence_iter& b_end, bool is_forward_b) const {
            while (!isEnd(end, it)) {
                seq.push_back(*it);
                ++it;
                if (is_forward_b) forwardUntilEnd(b_end, b_it);
            }
        }
        
    };
}

#endif // DTL_DIFF3_H
