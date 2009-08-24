/**
 dtl-1.02 -- Diff Template Library
 
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

#ifndef DTL_H
#define DTL_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>

namespace dtl {
  
  using std::vector;
  using std::string;
  using std::pair;
  using std::ostream;
  using std::list;
  using std::for_each;
  using std::distance;
  using std::fill;
  using std::cout;
  using std::endl;
  using std::rotate;
  using std::swap;
  using std::max;

  /**
   * type of edit for SES
   */
  typedef int edit_t;
  const   edit_t SES_DELETE = -1;
  const   edit_t SES_COMMON = 0;
  const   edit_t SES_ADD    = 1;

  /**
   * mark of SES
   */
  const string SES_MARK_DELETE = "-";
  const string SES_MARK_COMMON = " ";
  const string SES_MARK_ADD    = "+";

  /**
   * info for Unified Format
   */
  typedef struct eleminfo {
    int      beforeIdx;
    int      afterIdx;
    edit_t type;
  } elemInfo;

  #define DTL_SEPARATE_SIZE (3)
  #define DTL_CONTEXT_SIZE  (3)

  /**
   * cordinate for registering route
   */
  typedef struct Point {
    int x;
    int y;
    int k;
  } P;

  /**
   * limit of cordinate size
   */
  const unsigned int MAX_CORDINATES_SIZE = 2000000;
  
  typedef vector<int> editPath;
  typedef vector<P>   editPathCordinates;
  
  /**
   * Structure of Unified Format Hunk
   */
  template <typename sesElem>
  struct uniHunk {
    int a, b, c, d;              // @@ -a,b +c,d @@
    vector<sesElem> common[2];   // anteroposterior commons on changes
    vector<sesElem> change;      // changes
    int inc_dec_count;           // count of increace and decrease
  };
  
  /**
   * lcs with index
   */
  template <typename elem>
  struct idxLcs {
    elem e;
    int a_idx, b_idx;
  };
  
  /**
   * Functors
   */
  template <typename sesElem>
  class Print
  {
  public :
    Print ()             : out_(cout) {}
    Print (ostream& out) : out_(out)  {}
    virtual ~Print () {}
    virtual void operator() (const sesElem& se) const = 0;
  protected :
    ostream& out_;
  };
  
  template <typename sesElem>
  class PrintCommon : public Print < sesElem >
  {
  public :
    PrintCommon ()             : Print < sesElem > ()    {}
    PrintCommon (ostream& out) : Print < sesElem > (out) {}
    ~PrintCommon () {}
    void operator() (const sesElem& se) const {
      this->out_ << SES_MARK_COMMON << se.first << endl;    
    }
  };
  
  template <typename sesElem>
  class PrintChange : public Print < sesElem >
  {
  public :
    PrintChange ()             : Print < sesElem > ()    {}
    PrintChange (ostream& out) : Print < sesElem > (out) {}
    ~PrintChange () {}
    void operator() (const sesElem& se) const {
      switch (se.second.type) {
      case SES_ADD:
        this->out_ << SES_MARK_ADD    << se.first << endl;
        break;
      case SES_DELETE:
        this->out_ << SES_MARK_DELETE << se.first << endl;
        break;
      case SES_COMMON:
        this->out_ << SES_MARK_COMMON << se.first << endl;
        break;
      }
    }
  };
  
  template <typename sesElem>
  class PrintUniHunk
  {
  public :
    PrintUniHunk ()             : out_(cout) {}
    PrintUniHunk (ostream& out) : out_(out)  {}
    ~PrintUniHunk () {}
    void operator() (const uniHunk< sesElem >& hunk) const {
      out_ << "@@"
           << " -"  << hunk.a << "," << hunk.b
           << " +"  << hunk.c << "," << hunk.d
           << " @@" << endl;
      
      for_each(hunk.common[0].begin(), hunk.common[0].end(), PrintCommon< sesElem >());
      for_each(hunk.change.begin(),    hunk.change.end(),    PrintChange< sesElem >());
      for_each(hunk.common[1].begin(), hunk.common[1].end(), PrintCommon< sesElem >());
    }
  private :
    ostream& out_;
  };

  template <typename elem>
  class Compare
  {
  public :
    Compare () {}
    virtual ~Compare () {}
    virtual bool impl (const elem& e1, const elem& e2) const {
      return e1 == e2;
    }
  };
  
  /**
   * sequence template class
   */
  template <typename elem>
  class Sequence
  {
  public :
    typedef vector<elem> elemVec;
    Sequence () {}
    virtual ~Sequence () {}
    
    elemVec getSequence () const {
      return sequence;
    }
    void addSequence (elem e) {
      sequence.push_back(e);
    }
  protected :
    elemVec sequence;
  };
  
  /**
   * Longest Common Subsequence template calss
   */
  template <typename elem>
  class Lcs : public Sequence<elem>
  {
  private :
    typedef vector< idxLcs<elem> > lcsSequence;
    lcsSequence lcsSeq;
  public :
    Lcs ()  {}
    ~Lcs () {}
    void addLcsSequence (elem e, int a_idx, int b_idx) {
      idxLcs<elem> ie;
      ie.e = e;
      ie.a_idx = a_idx;
      ie.b_idx = b_idx;
      lcsSeq.push_back(ie);
    }
    lcsSequence getLcsSequence () const {
      return lcsSeq;
    }
  };

  /**
   * Shortest Edit Script template calss
   */
  template <typename elem>
  class Ses : public Sequence<elem>
  {
  private :
    typedef pair<elem, elemInfo> sesElem;
    typedef vector< sesElem > sesElemVec;
  public :
    
    Ses () : onlyAdd(true), onlyDelete(true), onlyCopy(true) { }
    ~Ses () {}
    
    bool isOnlyAdd () const {
      return onlyAdd;
    }
    
    bool isOnlyDelete () const {
      return onlyDelete;
    }
    
    bool isOnlyCopy () const {
      return onlyCopy;
    }
    
    bool isOnlyOneOperation () const {
      return isOnlyAdd() || isOnlyAdd() || isOnlyCopy();
    }
    
    bool isChange () const {
      return !onlyCopy;
    }

    using Sequence<elem>::addSequence;
    void addSequence (elem e, int beforeIdx, int afterIdx, const edit_t type) {
      elemInfo info;
      info.beforeIdx = beforeIdx;
      info.afterIdx  = afterIdx;
      info.type      = type;
      sesElem pe(e, info);
      sequence.push_back(pe);
      switch (type) {
      case SES_DELETE:
        onlyCopy   = false;
        onlyAdd    = false;
        break;
      case SES_COMMON:
        onlyAdd    = false;
        onlyDelete = false;
        break;
      case SES_ADD:
        onlyDelete = false;
        onlyCopy   = false;
        break;
      }
    }
    
    sesElemVec getSequence () const {
      return sequence;
    }
  private :
    sesElemVec sequence;
    bool onlyAdd;
    bool onlyDelete;
    bool onlyCopy;
  };

  /**
   * diff template class
   * sequence must support random_access_iterator.
   */
  template <typename elem, typename sequence>
  class Diff
  {
    typedef pair<elem, elemInfo> sesElem;
    typedef vector< sesElem > sesElemVec;
    typedef vector< uniHunk< sesElem > > uniHunkVec;
    typedef list< elem > elemList;
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
    Lcs<elem> lcs;
    Ses<elem> ses;
    editPath path;
    editPathCordinates pathCordinates;
    bool reverse;
    bool huge;
    bool unserious;
    bool onlyEditDistance;
    uniHunkVec uniHunks;
    Compare<elem> cmp;
  public :
    Diff (sequence& a, sequence& b) : A(a), B(b) {
      init();
    }

    Diff (sequence& a, sequence& b, Compare<elem>& comp) : A(a), B(b), cmp(comp) {
      init();
    }

    ~Diff() {}

    int getEditDistance () const {
      return editDistance;
    }

    Lcs<elem> getLcs () const {
      return lcs;
    }

    Ses<elem> getSes () const {
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
      for_each(ses_v.begin(), ses_v.end(), PrintChange< sesElem >());
    }

    static void printSES (Ses<elem>& s) {
      sesElemVec ses_v = s.getSequence();
      for_each(ses_v.begin(), ses_v.end(), PrintChange< sesElem >());
    }

    /**
     * print difference between A and B with the format such as Unified Format
     */
    void printUnifiedFormat () const {
      for_each(uniHunks.begin(), uniHunks.end(), PrintUniHunk< sesElem >());
    }
    
    static void printUnifiedFormat (uniHunkVec& hunks) {
      for_each(hunks.begin(), hunks.end(), PrintUniHunk< sesElem >());
    }

    /**
     * compose Unified Format Hunks from Shortest Edit Script
     */
    void composeUnifiedHunks () {
      sesElemVec common[2];
      sesElemVec change;
      sesElemVec ses_v = ses.getSequence();
      int l_cnt  = 1;
      int length = distance(ses_v.begin(), ses_v.end());
      int middle = 0;
      bool isMiddle, isAfter;
      isMiddle = isAfter = false;
      elem e;
      elemInfo einfo;
      int a, b, c, d;         // @@ -a,b +c,d @@
      int inc_dec_count = 0;
      a = b = c = d = 0;
      uniHunk<sesElem> hunk;
      sesElemVec adds;
      sesElemVec deletes;

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
          int cnt = 0;
          for (int i=0;i<DTL_SEPARATE_SIZE;++i, ++cit) {
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
            for (int i=0;i<c0size-DTL_SEPARATE_SIZE;++i) {
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

#endif // DTL_H
