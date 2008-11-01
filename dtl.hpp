/*
  dtl-0.02 -- Diff Template Library
  Copyright(C) 2008-  Tatsuhiko Kubo <cubicdaiya@gmail.com>
*/

#ifndef DTL_H
#define DTL_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>

namespace dtl {

  /**
   * type of edit for SES
   */
  typedef int editType;
  const   editType SES_DELETE = -1;
  const   editType SES_COMMON = 0;
  const   editType SES_ADD    = 1;

  /**
   * mark of SES
   */
  const std::string SES_MARK_DELETE = "-";
  const std::string SES_MARK_COMMON = " ";
  const std::string SES_MARK_ADD    = "+";

  /**
   * info for Unified Format
   */
  typedef struct eleminfo {
    int      beforeIdx;
    int      afterIdx;
    editType type;
  } elemInfo;

  /**
   * cordinate for registering route
   */
  typedef struct Point {
    int x;
    int y;
    int k;
  } P;

  const unsigned int MAX_CORDINATES_SITE = 2000000;
  
  template <typename elem>
  class Sequence
  {
  public :
    typedef std::vector<elem> _Sequence;
    Sequence () {
      
    }
    virtual ~Sequence () {
      
    }
    
    _Sequence getSequence () {
      return sequence;
    }
    void addSequence (elem e) {
      sequence.push_back(e);
    }
  protected :
    _Sequence sequence;
  };

  template <typename elem>
  class Lcs : public Sequence<elem>
  {
  public :
    Lcs ()  {}
    ~Lcs () {}
  };

  template <typename elem>
  class Ses : public Sequence<elem>
  {
  private :
    typedef std::pair<elem, elemInfo> sesElem;
    typedef std::vector< sesElem > sesSequence;
  public :
    
    Ses () {
      onlyAdd    = true;
      onlyDelete = true;
      onlyCopy   = true;
    }
    
    ~Ses () {}
    
    bool isOnlyAdd () {
      return onlyAdd;
    }
    
    bool isOnlyDelete () {
      return onlyDelete;
    }
    
    bool isOnlyCopy () {
      return onlyCopy;
    }
    
    bool isOnlyOneOperation () {
      return isOnlyAdd() || isOnlyAdd() || isOnlyCopy();
    }
    
    bool isChange () {
      return !onlyCopy;
    }
    
    using Sequence<elem>::addSequence;
    void addSequence (elem e, int beforeIdx, int afterIdx, editType type) {
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
    
    sesSequence getSequence () {
      return sequence;
    }
  private :
    sesSequence sequence;
    bool onlyAdd;
    bool onlyDelete;
    bool onlyCopy;
  };

  template <typename elem, typename sequence>
  class Diff
  {
  public :
    typedef std::vector<int> editPath;
    typedef std::vector<P>   editPathCordinates;
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
  public :
    Diff(sequence& A, sequence& B) {
      M = std::distance(A.begin(), A.end());
      N = std::distance(B.begin(), B.end());
      if (M < N) {
	this->A = A;
	this->B = B;
	reverse = false;
      } else {
	this->A = B;
	this->B = A;
	std::swap(M, N);
	reverse = true;
      }
      editDistance = 0;
      delta = N - M;
      offset = M + 1;
      int size = M + N + 3;
      fp = new int[size];
      std::fill(&fp[0], &fp[size], -1);
      path = editPath(size);
      std::fill(path.begin(), path.end(), -1);
    }
    
    ~Diff() {
      delete[] this->fp;
    }

    int getEditDistance() {
      return editDistance;
    }

    Lcs<elem> getLcs() {
      return lcs;
    }

    Ses<elem> getSes() {
      return ses;
    }

    bool isReverse () {
      return reverse;
    }

    sequence patch (sequence seq, Ses<elem>& ses) {
      std::vector< std::pair<elem, elemInfo> > sesSeq = ses.getSequence();
      std::list<elem> seqLst(seq.begin(), seq.end());
      std::list< std::pair<elem, elemInfo> > sesLst(sesSeq.begin(), sesSeq.end());
      typename std::list<elem>::iterator lstIt = seqLst.begin();
      typename std::vector< std::pair<elem, elemInfo> >::iterator sesIt;
      for (sesIt=sesSeq.begin();sesIt!=sesSeq.end();++sesIt) {
	switch (sesIt->second.type) {
	case SES_ADD :
	  seqLst.insert(lstIt, sesIt->first);
	  break;
	case SES_DELETE :
	  seqLst.erase(lstIt);
	  ++lstIt;
	  break;
	case SES_COMMON :
	  ++lstIt;
	  break;
	default :
	  break;
	}
      }
      sequence patchedSeq(seqLst.begin(), seqLst.end());
      return patchedSeq;
    }
    
    void compose() {
      // O(NP) Algorithm
      int p = -1;
      int k;
      do {
	++p;
	for (k=-p;k<=delta-1;++k) {
	  fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
	}
	for (k=delta+p;k>=delta+1;--k) {
	  fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
	}
	fp[delta+offset] = snake(delta, fp[delta-1+offset]+1, fp[delta+1+offset]);
      } while (fp[delta+offset] != N && pathCordinates.size() < MAX_CORDINATES_SITE);

      editDistance = delta + 2 * p;

      int r = path[delta+offset];
      P cordinate;
      editPathCordinates epc(0);
      while(r != -1){
	cordinate.x = pathCordinates[r].x;
	cordinate.y = pathCordinates[r].y;
	epc.push_back(cordinate);
	r = pathCordinates[r].k;
      }

      // recordSequence Longest Common Subsequence & Shortest Edit Script
      recordSequence(epc);
    }
    
  private :

    int snake(int k, int above, int below) {
      int r;
      if (above > below) {
	r = path[k-1+offset];
      } else {
	r = path[k+1+offset];
      }

      int y = std::max(above, below);
      int x = y - k;
      while (x < M && y < N && A[x] == B[y]) {
	++x;++y;
      }

      path[k+offset] = pathCordinates.size();
      P p;
      p.x = x;p.y = y;p.k = r;
      pathCordinates.push_back(p);      
      return y;
    }

    void recordSequence (editPathCordinates& v) {
      typename sequence::const_iterator x(A.begin());
      typename sequence::const_iterator y(B.begin());
      int x_idx, y_idx;   // line number for Unified Format
      int px_idx, py_idx; // cordinates
      int size = v.size() - 1;
      x_idx = y_idx = 1;
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
	// recoding on the way
	if (!isReverse()) {
	  recordOddSequence(x_idx, M, x, SES_DELETE);
	  recordOddSequence(y_idx, N, y, SES_ADD);
	} else {
	  recordOddSequence(x_idx, M, x, SES_ADD);
	  recordOddSequence(y_idx, N, y, SES_DELETE);
	}
      }
    }
    
    void recordOddSequence (int idx, int length, typename sequence::const_iterator it, const editType et) {
      while(idx < length){
	ses.addSequence(*it, idx, 0, et);
	++it;
	++idx;
	++editDistance;
      }
      ses.addSequence(*it, idx, 0, et);
      ++editDistance;
    }
  };
}

#endif
