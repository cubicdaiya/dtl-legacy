/**
 dtl-1.04 -- Diff Template Library
 
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

#ifndef DTL_DIFF_H
#define DTL_DIFF_H

namespace dtl {

  /**
   * diff class template
   * sequence must support random_access_iterator.
   */
  template <typename elem, typename sequence>
  class Diff
  {
    typedef pair< elem, elemInfo > sesElem;
    typedef vector< sesElem > sesElemVec;
    typedef vector< uniHunk< sesElem > > uniHunkVec;
    typedef list< elem > elemList;
    typedef vector< elem > elemVec;
    typedef typename uniHunkVec::iterator uniHunkVec_iter;
    typedef typename sesElemVec::iterator sesElemVec_iter;
    typedef typename elemList::iterator elemList_iter;
    typedef typename sequence::iterator sequence_iter;
    typedef typename sequence::const_iterator sequence_const_iter;
  private :
    sequence A;
    sequence B;
    int M;
    int N;
    int delta;
    int offset;
    int *fp;
    int editDistance;
    Lcs< elem > lcs;
    Ses< elem > ses;
    editPath path;
    editPathCordinates pathCordinates;
    bool reverse;
    bool huge;
    bool unserious;
    bool onlyEditDistance;
    uniHunkVec uniHunks;
    Compare<elem> cmp;
  public :
    Diff () {}

    Diff (sequence& a, sequence& b) : A(a), B(b) {
      init();
    }

    Diff (sequence& a, sequence& b, Compare< elem >& comp) : A(a), B(b), cmp(comp) {
      init();
    }

    ~Diff() {}

    int getEditDistance () const {
      return editDistance;
    }

    Lcs< elem > getLcs () const {
      return lcs;
    }

    elemVec getLcsVec () const {
      return lcs.getSequence();
    }

    Ses< elem > getSes () const {
      return ses;
    }

    uniHunkVec getUniHunks () const {
      return uniHunks;
    }

    bool isHuge () const {
      return huge;
    }

    void onHuge () {
      this->huge = true;
    }

    void offHuge () {
      this->huge = false;
    }
    
    bool isUnserious () const {
      return unserious;
    }

    void onUnserious () {
      this->unserious = true;
    }

    void offUnserious () {
      this->unserious = false;
    }
    
    void onOnlyEditDistance () {
      this->onlyEditDistance = true;
    }
    
    /**
     * patching with Unified Format Hunks
     */
    sequence uniPatch (sequence seq) {
      elemList seqLst(seq.begin(), seq.end());
      sesElemVec shunk;
      sesElemVec_iter vsesIt;
      elemList_iter lstIt   = seqLst.begin();
      elemList_iter lstIt_t = seqLst.begin();
      sequence_iter cit     = seq.begin();
      int inc_dec_total     = 0;
      int seq_lnum          = 1;
      int longer_seq_lnum   = 1;
      int loop              = 0;
      for (uniHunkVec_iter it=uniHunks.begin();it!=uniHunks.end();++it, ++loop) {
        joinSesVec(shunk, it->common[0]);
        joinSesVec(shunk, it->change);
        joinSesVec(shunk, it->common[1]);
        it->a += inc_dec_total;
        if (loop != 0) ++lstIt_t;
        lstIt = lstIt_t;
        while (seq_lnum++ < it->a && longer_seq_lnum++ < N) {
          ++cit;
          if (lstIt != seqLst.end()) ++lstIt;
        }
        lstIt_t = lstIt;
        inc_dec_total += it->inc_dec_count;
        vsesIt = shunk.begin();
        while (vsesIt!=shunk.end()) {
          switch (vsesIt->second.type) {
          case SES_ADD :
            seqLst.insert(lstIt, vsesIt->first);
            break;
          case SES_DELETE :
            if (lstIt != seqLst.end()) {
              lstIt = seqLst.erase(lstIt);
            }
            break;
          case SES_COMMON :
            if (lstIt != seqLst.end()) {
              ++lstIt;
            }
            break;
          default :
            break;
          }
          ++vsesIt;
        }
        shunk.clear();
      }
      
      sequence patchedSeq(seqLst.begin(), seqLst.end());
      return patchedSeq;
    }
    
    /**
     * patching with Shortest Edit Script
     */
    sequence patch (sequence seq) const {
      sesElemVec sesSeq = ses.getSequence();
      elemList seqLst(seq.begin(), seq.end());
      elemList_iter lstIt = seqLst.begin();
      for (sesElemVec_iter sesIt=sesSeq.begin();sesIt!=sesSeq.end();++sesIt) {
        switch (sesIt->second.type) {
        case SES_ADD :
          seqLst.insert(lstIt, sesIt->first);
          break;
        case SES_DELETE :
          lstIt = seqLst.erase(lstIt);
          break;
        case SES_COMMON :
          ++lstIt;
          break;
        default :
          // no through
          break;
        }
      }
      sequence patchedSeq(seqLst.begin(), seqLst.end());
      return patchedSeq;
    }
    
    /**
     * compose Longest Common Subsequence and Shortest Edit Script.
     * The algorithm implemented here is based on "An O(NP) Sequence Comparison Algorithm"
     * by described by Sun Wu, Udi Manber and Gene Myers
     */
    void compose() {

      if (isHuge()) pathCordinates.reserve(MAX_CORDINATES_SIZE + 50000);

      int p = -1;
      int size = M + N + 3;
      fp = new int[size];
      fill(&fp[0], &fp[size], -1);
      path = editPath(size);
      fill(path.begin(), path.end(), -1);
    ONP:
      do {
        ++p;
        for (int k=-p;k<=delta-1;++k) {
          fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
        }
        for (int k=delta+p;k>=delta+1;--k) {
          fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
        }
        fp[delta+offset] = snake(delta, fp[delta-1+offset]+1, fp[delta+1+offset]);
      } while (fp[delta+offset] != N && pathCordinates.size() < MAX_CORDINATES_SIZE);

      editDistance += delta + 2 * p;
      int r = path[delta+offset];
      P cordinate;
      editPathCordinates epc(0);
      
      // only recoding editdistance
      if (onlyEditDistance) return;

      while(r != -1){
        cordinate.x = pathCordinates[r].x;
        cordinate.y = pathCordinates[r].y;
        epc.push_back(cordinate);
        r = pathCordinates[r].k;
      }

      // record Longest Common Subsequence & Shortest Edit Script
      if (!recordSequence(epc)) {
        pathCordinates.resize(0);
        epc.resize(0);
        p = -1;
        goto ONP;
      }
      delete[] this->fp;
    }

    /**
     * print difference between A and B with SES
     */
    void printSES () const {
      sesElemVec ses_v = ses.getSequence();
      for_each(ses_v.begin(), ses_v.end(), ChangePrinter< sesElem >());
    }

    /**
     * print difference with gived SES
     */
    static void printSES (Ses< elem >& s) {
      sesElemVec ses_v = s.getSequence();
      for_each(ses_v.begin(), ses_v.end(), ChangePrinter< sesElem >());
    }

    /**
     * print difference between A and B with the format such as Unified Format
     */
    void printUnifiedFormat () const {
      for_each(uniHunks.begin(), uniHunks.end(), UniHunkPrinter< sesElem >());
    }
    
    /**
     * print unified format difference with gived unified format hunks
     */
    static void printUnifiedFormat (uniHunkVec& hunks) {
      for_each(hunks.begin(), hunks.end(), UniHunkPrinter< sesElem >());
    }

    /**
     * compose Unified Format Hunks from Shortest Edit Script
     */
    void composeUnifiedHunks () {
      sesElemVec common[2];
      sesElemVec change;
      sesElemVec ses_v = ses.getSequence();
      int l_cnt        = 1;
      int length       = distance(ses_v.begin(), ses_v.end());
      uint middle      = 0;
      bool isMiddle, isAfter;
      elem e;
      elemInfo einfo;
      int a, b, c, d;         // @@ -a,b +c,d @@
      int inc_dec_count = 0;
      uniHunk<sesElem> hunk;
      sesElemVec adds;
      sesElemVec deletes;

      isMiddle = isAfter = false;
      a = b = c = d = 0;

      for (sesElemVec_iter it=ses_v.begin();it!=ses_v.end();++it, ++l_cnt) {
        e = it->first;
        einfo = it->second;
        switch (einfo.type) {
        case SES_ADD :
          middle = 0;
          ++inc_dec_count;
          adds.push_back(*it);
          if (!isMiddle)       isMiddle = true;
          if (isMiddle)        ++d;
          if (l_cnt >= length) {
            joinSesVec(change, deletes);
            joinSesVec(change, adds);
            isAfter = true;
          }
          break;
        case SES_DELETE :
          middle = 0;
          --inc_dec_count;
          deletes.push_back(*it);
          if (!isMiddle)       isMiddle = true;
          if (isMiddle)        ++b;
          if (l_cnt >= length) {
            joinSesVec(change, deletes);
            joinSesVec(change, adds);
            isAfter = true;
          }
          break;
        case SES_COMMON :
          ++b;++d;
          if (common[1].empty() && adds.empty() && deletes.empty() && change.empty()) {
            if (common[0].size() < DTL_CONTEXT_SIZE) {
              if (a == 0 && c == 0) {
                a = einfo.beforeIdx;
                c = einfo.afterIdx;
              }
              common[0].push_back(*it);
            } else {
              rotate(common[0].begin(), common[0].begin() + 1, common[0].end());
              common[0].pop_back();
              common[0].push_back(*it);
              ++a;++c;
              --b;--d;
            }
          }
          if (isMiddle && !isAfter) {
            ++middle;
            joinSesVec(change, deletes);
            joinSesVec(change, adds);
            change.push_back(*it);
            if (middle >= DTL_SEPARATE_SIZE || l_cnt >= length) {
              isAfter = true;
            }
            adds.clear();
            deletes.clear();
          }
          break;
        default :
          // no through
          break;
        }
        // compose unified format hunk
        if (isAfter && !change.empty()) {
          sesElemVec_iter cit = it;
          uint cnt = 0;
          for (int i=0;i<(int)DTL_SEPARATE_SIZE;++i, ++cit) {
            if (cit->second.type == SES_COMMON) {
              ++cnt;
            }
          }
          if (cnt < DTL_SEPARATE_SIZE && l_cnt < length) {
            middle = 0;
            isAfter = false;
            continue;
          }
          if (common[0].size() >= DTL_SEPARATE_SIZE) {
            int c0size = common[0].size();
            rotate(common[0].begin(), 
                   common[0].begin() + c0size - DTL_SEPARATE_SIZE, 
                   common[0].end());
            for (int i=0;i<c0size-(int)DTL_SEPARATE_SIZE;++i) {
              common[0].pop_back();
            }
            a += c0size - DTL_SEPARATE_SIZE;
            c += c0size - DTL_SEPARATE_SIZE;
          }
          if (a == 0) ++a;
          if (c == 0) ++c;
          if (isReverse()) swap(a, c);
          hunk.a = a;hunk.b = b;hunk.c = c;hunk.d = d;
          hunk.common[0] = common[0];
          hunk.change = change;
          hunk.common[1] = common[1];
          hunk.inc_dec_count = inc_dec_count;
          uniHunks.push_back(hunk);
          isMiddle = false;
          isAfter = false;
          common[0].clear();
          common[1].clear();
          adds.clear();
          deletes.clear();
          change.clear();
          a = b = c = d = middle = inc_dec_count = 0;
        }
      }
    }
  private :
    void init () {
      M = distance(A.begin(), A.end());
      N = distance(B.begin(), B.end());
      if (M < N) {
        reverse = false;
      } else {
        swap(A, B);
        swap(M, N);
        reverse = true;
      }
      editDistance     = 0;
      delta            = N - M;
      offset           = M + 1;
      huge             = false;
      unserious        = false;
      onlyEditDistance = false;
      fp               = NULL;
    }
    
    int snake(int k, int above, int below) {
      int r;
      if (above > below) {
        r = path[k-1+offset];
      } else {
        r = path[k+1+offset];
      }

      int y = max(above, below);
      int x = y - k;
      while (x < M && y < N && cmp.impl(A[x], B[y])) {
        ++x;++y;
      }

      path[k+offset] = pathCordinates.size();
      if (!onlyEditDistance) {
        P p;
        p.x = x;p.y = y;p.k = r;
        pathCordinates.push_back(p);      
      }
      return y;
    }
    
    /**
     * record SES and LCS
     */
    bool recordSequence (editPathCordinates& v) {
      sequence_const_iter x(A.begin());
      sequence_const_iter y(B.begin());
      int x_idx, y_idx;   // line number for Unified Format
      int px_idx, py_idx; // cordinates
      int size = v.size() - 1;
      x_idx  = y_idx  = 1;
      px_idx = py_idx = 0;
      for (int i=size;i>=0;--i) {
        while(px_idx < v[i].x || py_idx < v[i].y) {
          if (v[i].y - v[i].x > py_idx - px_idx) {
            if (!isReverse()) {
              ses.addSequence(*y, y_idx, 0, SES_ADD);
            } else {
              ses.addSequence(*y, y_idx, 0, SES_DELETE);
            }
            ++y;
            ++y_idx;
            ++py_idx;
          } else if (v[i].y - v[i].x < py_idx - px_idx) {
            if (!isReverse()) {
              ses.addSequence(*x, x_idx, 0, SES_DELETE);
            } else {
              ses.addSequence(*x, x_idx, 0, SES_ADD);
            }
            ++x;
            ++x_idx;
            ++px_idx;
          } else {             // Common
            lcs.addSequence(*x);
            lcs.addLcsSequence(*x, x_idx, y_idx);
            ses.addSequence(*x, x_idx, y_idx, SES_COMMON);
            ++x;
            ++y;
            ++x_idx;
            ++y_idx;
            ++px_idx;
            ++py_idx;
          }
        }
      }

      if (x_idx > M && y_idx > N) {
        // all recording success
      } else {
        // unserious difference
        if (isUnserious()) {
          if (!isReverse()) {
            recordOddSequence(x_idx, M, x, SES_DELETE);
            recordOddSequence(y_idx, N, y, SES_ADD);
          } else {
            recordOddSequence(x_idx, M, x, SES_ADD);
            recordOddSequence(y_idx, N, y, SES_DELETE);
          }
          return true;
        }
        
        // decent difference
        sequence A_(A.begin() + x_idx - 1, A.end());
        sequence B_(B.begin() + y_idx - 1, B.end());
        A        = A_;
        B        = B_;
        M        = distance(A.begin(), A.end());
        N        = distance(B.begin(), B.end());
        delta    = N - M;
        offset   = M + 1;
        int size = M + N + 3;
        delete[] fp;
        fp = new int[size];
        fill(&fp[0], &fp[size], -1);
        fill(path.begin(), path.end(), -1);
        return false;
      }
      return true;
    }

    /**
     * record odd sequence to ses
     */
    void recordOddSequence (int idx, int length, sequence_const_iter it, const edit_t et) {
      while(idx < length){
        ses.addSequence(*it, idx, 0, et);
        ++it;
        ++idx;
        ++editDistance;
      }
      ses.addSequence(*it, idx, 0, et);
      ++editDistance;
    }

    /**
     * join ses vectors
     */
    void joinSesVec (sesElemVec& s1, sesElemVec& s2) const {
      if (!s2.empty()) {
        for (sesElemVec_iter vit=s2.begin();vit!=s2.end();++vit) {
          s1.push_back(*vit);
        }
      }      
    }

    bool isReverse () const {
      return reverse;
    }
  };
}

#endif // DTL_DIFF_H
