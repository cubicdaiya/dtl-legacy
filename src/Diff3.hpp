/**
 dtl-1.03 -- Diff Template Library
 
 In short, Diff Template Library is distributed under so called "BSD license",
 
 Copyright (c) 2008-2009 Tatsuhiko Kubo <cubicdaiya@gmail.com>
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
   * diff3 template class
   * sequence must support random_access_iterator.
   */
  template <typename elem, typename sequence>
  class Diff3
  {
    typedef pair< elem, elemInfo > sesElem;
    typedef vector< sesElem > sesElemVec;
    typedef vector< elem > elemVec;
    typedef typename sesElemVec::iterator sesElemVec_iter;
  private:
    sequence A;
    sequence B;
    sequence C;
    sequence S;
    Diff<elem, sequence> diff_ba;
    Diff<elem, sequence> diff_bc;
    bool conflict;
    elem csepabegin;
    elem csepa1;
    elem csepa2;
    elem csepaend;
  public :
    Diff3 (sequence& a, sequence& b, sequence& c) : A(a), B(b), C(c), 
                                                    diff_ba(b, a), diff_bc(b, c), 
                                                    conflict(false) {} 

    ~Diff3 () {}
    
    bool isConflict () const {
      return conflict;
    }
    
    void setConflictSeparators (elem begin, elem sepa1, elem sepa2, elem end) {
      csepabegin = begin;
      csepa1     = sepa1;
      csepa2     = sepa2;
      csepaend   = end;
    }

    sequence getMergedSequence () const {
      return S;
    }

    // merge changes B and C to A
    bool merge () {
      if (diff_ba.getEditDistance() == 0) {   // A == B
        if (diff_bc.getEditDistance() == 0) { // A == B == C
          S = B;
          return true;
        }
        S = C;
        return true;
      } else {                                // A != B
        if (diff_bc.getEditDistance() == 0) { // A != B == C
          S = A;                              
          return true;
        } else {                              // A != B != C
          S = merge_();
          if (isConflict()) {                 // conflict occured
            specifyConfliction();
            return false;
          }
        }
      }
      return true;
    }

    void compose () {
      diff_ba.compose();
      diff_bc.compose();
    }
    
  private :
    sequence merge_ () {
      elemVec seq;
      Ses<elem> ses_ba       = diff_ba.getSes();
      Ses<elem> ses_bc       = diff_bc.getSes();
      sesElemVec ses_ba_v    = ses_ba.getSequence();
      sesElemVec ses_bc_v    = ses_bc.getSequence();
      sesElemVec_iter ba_it  = ses_ba_v.begin();
      sesElemVec_iter bc_it  = ses_bc_v.begin();
      sesElemVec_iter ba_end = ses_ba_v.end();
      sesElemVec_iter bc_end = ses_bc_v.end();
      bool is_ba_end         = false;
      bool is_bc_end         = false;
      while (ba_it != ba_end || bc_it != ba_end) {
        setEndFlag(ba_end, ba_it, is_ba_end);
        setEndFlag(bc_end, bc_it, is_bc_end);
        if (is_ba_end || is_bc_end) break;
        while (true) {
          if (   ba_it != ba_end
              && bc_it != bc_end
              && ba_it->first == bc_it->first 
              && ba_it->second.type == SES_COMMON 
              && bc_it->second.type == SES_COMMON) {
            // do nothing
          } else {
            break;
          }
          if      (ba_it != ba_end) seq.push_back(ba_it->first);
          else if (bc_it != bc_end) seq.push_back(bc_it->first);
          forwardUntilEnd(ba_end, ba_it);
          forwardUntilEnd(bc_end, bc_it);
        }
        setEndFlag(ba_end, ba_it, is_ba_end);
        setEndFlag(bc_end, bc_it, is_bc_end);
        if (is_ba_end || is_bc_end) break;
        if (ba_it->second.type == SES_COMMON && bc_it->second.type == SES_DELETE) {
          forwardUntilEnd(ba_end, ba_it);
          forwardUntilEnd(bc_end, bc_it);
        } else if (ba_it->second.type == SES_COMMON && bc_it->second.type == SES_ADD) {
          seq.push_back(bc_it->first);
          forwardUntilEnd(bc_end, bc_it);
        } else if (ba_it->second.type == SES_DELETE && bc_it->second.type == SES_COMMON) {
          forwardUntilEnd(ba_end, ba_it);
          forwardUntilEnd(bc_end, bc_it);
        } else if (ba_it->second.type == SES_DELETE && bc_it->second.type == SES_DELETE) {
          if (ba_it->first == bc_it->first) {
            forwardUntilEnd(ba_end, ba_it);
            forwardUntilEnd(bc_end, bc_it);
          } else {
            // conflict
            conflict = true;
            return B;
          }
        } else if (ba_it->second.type == SES_DELETE && bc_it->second.type == SES_ADD) {
          // conflict
          conflict = true;
          return B;
        } else if (ba_it->second.type == SES_ADD && bc_it->second.type == SES_COMMON) {
          seq.push_back(ba_it->first);
          forwardUntilEnd(ba_end, ba_it);
        } else if (ba_it->second.type == SES_ADD && bc_it->second.type == SES_DELETE) {
          // conflict
          conflict = true;
          return B;
        } else if (ba_it->second.type == SES_ADD && bc_it->second.type == SES_ADD) {
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

      if (is_ba_end) {
        addDecentSequence(bc_end, bc_it, seq);
      } else if (is_bc_end) {
        addDecentSequence(ba_end, ba_it, seq);
      }

      sequence mergedSeq(seq.begin(), seq.end());
      return mergedSeq;
    }
    
    void specifyConfliction () {
      // not implement
    }
    
    void forwardUntilEnd (const sesElemVec_iter& end, sesElemVec_iter& it) const {
      if (it != end) ++it;
    }

    void setEndFlag (const sesElemVec_iter& end, sesElemVec_iter& it, bool& b) const {
      if (it == end) b = true;
    }

    void addDecentSequence (const sesElemVec_iter& end, sesElemVec_iter& it, elemVec& seq) const {
      while (it != end) {
        if (it->second.type == SES_ADD) seq.push_back(it->first);
        ++it;
      }      
    }
  };
}

#endif // DTL_DIFF3_H
