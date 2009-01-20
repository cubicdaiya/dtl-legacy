/**
 * dtl-0.06 -- Diff Template Library
 * Copyright(C) 2008-2009  Tatsuhiko Kubo <cubicdaiya@gmail.com>
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

  #define SEPARATE_SIZE (3)
  #define CONTEXT_SIZE (3)

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
  
  typedef std::vector<int> editPath;
  typedef std::vector<P>   editPathCordinates;
  
  /**
   * Structure of Unified Format Hunk
   */
  template <typename sesElem>
  struct uniHunk {
    int a, b, c, d;                   // @@ -a,b +c,d @@
    std::vector<sesElem> common[2];   // anteroposterior commons on changes
    std::vector<sesElem> change;      // changes
    int inc_dec_count;                // count of increace and decrease
  };

  /**
   * sequence template class
   */
  template <typename elem>
  class Sequence
  {
  public :
    typedef std::vector<elem> _Sequence;
    Sequence () {}
    virtual ~Sequence () {}

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
  struct idxLcs {
    elem e;
    int a_idx, b_idx;
  };

  /**
   * Longest Common Subsequence template calss
   */
  template <typename elem>
  class Lcs : public Sequence<elem>
  {
  private :
    typedef std::vector< idxLcs<elem> > lcsSequence;
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
    lcsSequence getLcsSequence () {
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

  /**
   * diff template class
   */
  template <typename elem, typename sequence>
  class Diff
  {
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
    typedef std::pair<elem, elemInfo> sesElem;
    std::vector< uniHunk<sesElem> > uniHunks;
    std::vector<int> change_idxes;
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
      change_idxes.reserve(N);
      change_idxes.resize(N);
      std::fill(change_idxes.begin(), change_idxes.end(), SES_COMMON);
      huge = false;
      unserious = false;
    }

    ~Diff() {
      delete[] this->fp;
    }

    int getEditDistance () {
      return editDistance;
    }

    Lcs<elem> getLcs () {
      return lcs;
    }

    Ses<elem> getSes () {
      return ses;
    }

    std::vector<int> getChangeIdx () {
      return change_idxes;
    }

    std::vector< uniHunk<sesElem> > getUniHunks () {
      return uniHunks;
    }

    bool isReverse () {
      return reverse;
    }

    bool isHuge () {
      return huge;
    }

    void onHuge () {
      this->huge = true;
    }

    void offHuge () {
      this->huge = false;
    }
    
    bool isUnserious () {
      return unserious;
    }

    void onUnserious () {
      this->unserious = true;
    }

    void offUnserious () {
      this->unserious = false;
    }
    
    /**
     * patching with Unified Format Hunks
     */
    sequence uniPatch (sequence seq) {
      std::list<elem> seqLst(seq.begin(), seq.end());
      std::vector<sesElem> shunk;
      typename std::vector< uniHunk<sesElem> >::iterator it;
      typename std::list<elem>::iterator lstIt = seqLst.begin();
      typename std::list<elem>::iterator lstIt_t = seqLst.begin();;
      typename std::vector<sesElem>::iterator vsesIt;
      typename sequence::iterator cit = seq.begin();
      int inc_dec_total = 0;
      int seq_lnum = 1;
      int longer_seq_lnum = 1;
      int loop = 0;
      for (it=uniHunks.begin();it!=uniHunks.end();++it, ++loop) {
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
    sequence patch (sequence seq, Ses<elem>& ses) {
      std::vector<sesElem> sesSeq = ses.getSequence();
      std::list<elem> seqLst(seq.begin(), seq.end());
      std::list<sesElem> sesLst(sesSeq.begin(), sesSeq.end());
      typename std::list<elem>::iterator lstIt = seqLst.begin();
      typename std::vector<sesElem>::iterator sesIt;
      for (sesIt=sesSeq.begin();sesIt!=sesSeq.end();++sesIt) {
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
	  break;
	}
      }
      sequence patchedSeq(seqLst.begin(), seqLst.end());
      return patchedSeq;
    }
    
    /**
     * compose Longest Common Subsequence and Shortest Edit Script.
     * The Algorithm of diff is O(NP).
     */
    void compose() {

      if (isHuge()) pathCordinates.reserve(MAX_CORDINATES_SIZE + 50000);

      // O(NP) Algorithm
      int p = -1;
      int k;
    ONP:
      do {
	++p;
	for (k=-p;k<=delta-1;++k) {
	  fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
	}
	for (k=delta+p;k>=delta+1;--k) {
	  fp[k+offset] = snake(k, fp[k-1+offset]+1, fp[k+1+offset]);
	}
	fp[delta+offset] = snake(delta, fp[delta-1+offset]+1, fp[delta+1+offset]);
      } while (fp[delta+offset] != N && pathCordinates.size() < MAX_CORDINATES_SIZE);

      editDistance += delta + 2 * p;
      int r = path[delta+offset];
      P cordinate;
      editPathCordinates epc(0);

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
    }

    /**
     * print difference between A and B with the format such as Unified Format.
     */
    void printUnifiedFormat () {
      typename std::vector< uniHunk<sesElem> >::iterator uit;
      for (uit=uniHunks.begin();uit!=uniHunks.end();++uit) {
	// header
	std::cout << "@@" 
		  << " -" << uit->a << "," << uit->b 
		  << " +" << uit->c << "," << uit->d 
		  << " @@" << std::endl;

	// header commons
	typename std::vector<sesElem>::iterator vit;
	for (vit=uit->common[0].begin();vit!=uit->common[0].end();++vit) {
	  std::cout << SES_MARK_COMMON << vit->first << std::endl;
	}
	// changes
	for (vit=uit->change.begin();vit!=uit->change.end();++vit) {
	  switch (vit->second.type) {
	  case SES_ADD:
	    std::cout << SES_MARK_ADD    << vit->first << std::endl;
	    break;
	  case SES_DELETE:
	    std::cout << SES_MARK_DELETE << vit->first << std::endl;
	    break;
	  case SES_COMMON:
	    std::cout << SES_MARK_COMMON << vit->first << std::endl;
	    break;
	  }
	}
	// footer commons
	for (vit=uit->common[1].begin();vit!=uit->common[1].end();++vit) {
	  std::cout << " " << vit->first << std::endl;
	}
      }
    }
    
    /**
     * compose Unified Format Hunks from Shortest Edit Script
     */
    void composeUnifiedHunks () {
      std::vector<sesElem> common[2];
      std::vector<sesElem> change;
      std::vector<sesElem> ses_v = ses.getSequence();
      typename std::vector<sesElem>::iterator it;
      it = ses_v.begin();
      int l_cnt = 1;
      int length = std::distance(ses_v.begin(), ses_v.end());
      int middle = 0;
      bool isMiddle, isAfter;
      isMiddle = isAfter = false;
      elem e;
      elemInfo einfo;
      int a, b, c, d;         // @@ -a,b +c,d @@
      int inc_dec_count = 0;
      a = b = c = d = 0;
      uniHunk<sesElem> hunk;
      std::vector<sesElem> adds;
      std::vector<sesElem> deletes;

      for (it=ses_v.begin();it!=ses_v.end();++it, ++l_cnt) {
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
	    if (common[0].size() < CONTEXT_SIZE) {
	      if (a == 0 && c == 0) {
		a = einfo.beforeIdx;
		c = einfo.afterIdx;
	      }
	      common[0].push_back(*it);
	    } else {
	      std::rotate(common[0].begin(), common[0].begin() + 1, common[0].end());
	      common[0].pop_back();
	      common[0].push_back(*it);
	      ++a;++c;
	      --b;--d;
	    }
	  }
	  if (isMiddle && !isAfter) {
	    ++middle;
	    typename std::vector<sesElem>::iterator vit;
	    joinSesVec(change, deletes);
	    joinSesVec(change, adds);
	    change.push_back(*it);
	    if (middle >= SEPARATE_SIZE || l_cnt >= length) {
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
	  typename std::vector<sesElem>::iterator cit = it;
	  int cnt = 0;
	  for (int i=0;i<SEPARATE_SIZE;++i, ++cit) {
	    if (cit->second.type == SES_COMMON) {
	      ++cnt;
	    }
	  }
	  if (cnt < SEPARATE_SIZE && l_cnt < length) {
	    middle = 0;
	    isAfter = false;
	    continue;
	  }
	  if (common[0].size() >= SEPARATE_SIZE) {
	    int c0size = common[0].size();
	    std::rotate(common[0].begin(), 
			common[0].begin() + c0size - SEPARATE_SIZE, 
			common[0].end());
	    for (int i=0;i<c0size-SEPARATE_SIZE;++i) {
	      common[0].pop_back();
	    }
	    a += c0size - SEPARATE_SIZE;
	    c += c0size - SEPARATE_SIZE;
	  }
	  if (a == 0) ++a;
	  if (c == 0) ++c;
	  if (isReverse()) std::swap(a, c);
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

    bool recordSequence (editPathCordinates& v) {
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
	      change_idxes[py_idx] = SES_ADD;
	    } else {
	      ses.addSequence(*y, y_idx, 0, SES_DELETE);
	      change_idxes[py_idx] = SES_DELETE;
	    }
	    ++y;
	    ++y_idx;
	    ++py_idx;
	  } else if (v[i].y - v[i].x < py_idx - px_idx) {
	    if (!isReverse()) {
	      ses.addSequence(*x, x_idx, 0, SES_DELETE);
	      change_idxes[px_idx] = SES_DELETE;
	    } else {
	      ses.addSequence(*x, x_idx, 0, SES_ADD);
	      change_idxes[px_idx] = SES_ADD;
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
	A = A_;
	B = B_;
	M = std::distance(A.begin(), A.end());
	N = std::distance(B.begin(), B.end());
	delta = N - M;
	offset = M + 1;
	int size = M + N + 3;
	delete[] fp;
	fp = new int[size];
	std::fill(&fp[0], &fp[size], -1);
	std::fill(path.begin(), path.end(), -1);
	return false;
      }
      return true;
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

    void joinSesVec (std::vector<sesElem>& s1, std::vector<sesElem>& s2) {
      typename std::vector<sesElem>::iterator vit;
      if (!s2.empty()) {
	for (vit=s2.begin();vit!=s2.end();++vit) {
	  s1.push_back(*vit);
	}
      }      
    }

  };

  /* 
   * diff3 template
   */
  template <typename elem, typename sequence>
  class Diff3
  {
  private:
    Diff<elem, sequence> *diff_ba;
    Diff<elem, sequence> *diff_bc;
    sequence A;
    sequence B;
    sequence C;
    sequence S;                                     // merged sequence
    bool conflict;
  public :
    Diff3 (sequence& A, sequence& B, sequence& C) {
      this->A = A;
      this->B = B;
      this->C = C;
      this->diff_ba = new Diff<elem, sequence>(B, A);
      this->diff_bc = new Diff<elem, sequence>(B, C);
      conflict = false;
    } 
    ~Diff3 () {
      delete this->diff_ba;
      delete this->diff_bc;
    }

    bool isConflict () {
      return conflict;
    }

    Diff<elem, sequence> getDiffba () {
      return diff_ba;
    }

    Diff<elem, sequence> getDiffbc () {
      return diff_bc;
    }
    
    sequence getMergedSequence () {
      return S;
    }

    // merge changes B and C to A
    bool merge () {
      if (diff_ba->getEditDistance() == 0) {   // A == B
	if (diff_bc->getEditDistance() == 0) { // A == B == C
	  S = B;
	  return true;
	}
	S = C;
	return true;
      } else {                                 // A != B
	if (diff_bc->getEditDistance() == 0) { // B == C
	  S = A;                              
	  return true;
	} else {
	  // A != B != C
	  if (isConflict_()) {
	    conflict = true;
	    return false;
	  }
	  S = merge_();
	}
      }
      return true;
    }

    void compose () {
      diff_ba->compose();
      diff_bc->compose();
    }
    
  private :
    sequence merge_ () {
      Ses<elem> ses_ba = diff_ba->getSes();
      Ses<elem> ses_bc = diff_bc->getSes();
      typedef std::vector< std::pair<elem, elemInfo> > ses_v;
      ses_v sv_ba = ses_ba.getSequence();
      ses_v sv_bc = ses_bc.getSequence();
      typename ses_v::iterator it_ba;
      typename ses_v::iterator it_bc;

      // LCS
      Lcs<elem> lcs_ba = diff_ba->getLcs();
      std::vector<elem> lcs_ba_v = lcs_ba.getSequence();
      std::string lcs_ba_s(lcs_ba_v.begin(), lcs_ba_v.end());
      std::vector< idxLcs<elem> > lcsSequence_ba = lcs_ba.getLcsSequence();
      Lcs<elem> lcs_bc = diff_bc->getLcs();
      std::vector<elem> lcs_bc_v = lcs_bc.getSequence();
      std::string lcs_bc_s(lcs_bc_v.begin(), lcs_bc_v.end());
      std::vector< idxLcs<elem> > lcsSequence_bc = lcs_bc.getLcsSequence();

      typename std::vector< idxLcs<elem> >::iterator lcs_ba_it;
      typename std::vector< idxLcs<elem> >::iterator lcs_bc_it;
      
      std::cout << "ba:" << std::endl;
      std::cout << "LCS_ba:" << lcs_ba_s << std::endl;
      for (lcs_ba_it=lcsSequence_ba.begin();lcs_ba_it!=lcsSequence_ba.end();++lcs_ba_it) {
	std::cout << "b_idx:" << lcs_ba_it->a_idx << ","
		  << "a_idx:" << lcs_ba_it->b_idx << ","
		  << "e:"     << lcs_ba_it->e     << std::endl;
      }
      
      std::cout << "bc:" << std::endl; 
      std::cout << "LCS_bc:" << lcs_bc_s << std::endl;
      for (lcs_bc_it=lcsSequence_bc.begin();lcs_bc_it!=lcsSequence_bc.end();++lcs_bc_it) {
	std::cout << "b_idx:" << lcs_bc_it->a_idx << ","
		  << "c_idx:" << lcs_bc_it->b_idx << ","
		  << "e:"     << lcs_bc_it->e     << std::endl;
      }
      
      lcs_ba_it = lcsSequence_ba.begin();
      lcs_bc_it = lcsSequence_bc.begin();
      std::vector<elem> seq;
      int offset_ba, offset_bc;
      int a_idx, b_idx, c_idx;
      int a_len, b_len, c_len;
      a_idx = b_idx = c_idx = 0;
      a_len = A.size();
      b_len = B.size();
      c_len = C.size();
      offset_ba = offset_bc = 0;
      while (!(a_idx >= a_len && b_idx >= b_len && c_idx >= c_len)) {
	if (lcs_ba_it->b_idx == lcs_ba_it->a_idx) { // lcs between B and A
	  if (lcs_bc_it->b_idx == lcs_bc_it->a_idx) { // lcs between B and C
	    if (lcs_ba_it->e == lcs_bc_it->e) {
	      std::cout << "1:" << a_idx << ":" << A[a_idx] << ",e:" << lcs_ba_it->e << std::endl;
	      if (lcs_ba_it != lcsSequence_ba.end()) {
		seq.push_back(lcs_ba_it->e);
	      } else if (c_idx < c_len - 1) {
		seq.push_back(C[c_idx]);
	      } else if (a_idx < a_len - 1) {
		seq.push_back(A[a_idx]);
	      }
	      ++a_idx;++b_idx;++c_idx;++lcs_ba_it;++lcs_bc_it;
	    } else { // conflict
	      
	    }
	  } else {
	    if (lcs_bc_it->b_idx <= lcs_bc_it->a_idx + offset_bc) {
	      if (lcs_ba_it->e == lcs_bc_it->e) {
		if (lcs_ba_it != lcsSequence_ba.end()) {
		  seq.push_back(lcs_ba_it->e);
		} else if (c_idx < c_len - 1) {
		  seq.push_back(C[c_idx]);
		} else if (a_idx < a_len - 1) {
		  seq.push_back(A[a_idx]);
		}
		std::cout << "2:" << a_idx << ":" << A[a_idx] << std::endl;
		++a_idx;++b_idx;++c_idx;++lcs_ba_it;++lcs_bc_it;
	      }
	    } else {
	      if (c_idx < c_len - 1) seq.push_back(C[c_idx]);
	      ++c_idx;++b_idx;++offset_bc;
	    }
	  }
	} else {
	  if (lcs_ba_it->b_idx <= lcs_ba_it->a_idx + offset_ba) {
	    if (lcs_ba_it->e == lcs_bc_it->e) {
	      if (lcs_ba_it != lcsSequence_ba.end()) {
		seq.push_back(lcs_bc_it->e);
	      } else if (c_idx < c_len - 1) {
		seq.push_back(C[c_idx]);
	      } else if (a_idx < a_len - 1) {
		seq.push_back(A[a_idx]);
	      }
	      std::cout << "3:" << c_idx << ":" << C[c_idx] << std::endl;
	      ++a_idx;++b_idx;++c_idx;++lcs_ba_it;++lcs_bc_it;
	    }
	  } else {
	    if (a_idx < a_len - 1) seq.push_back(A[a_idx]);
	    std::cout << "4:" << a_idx << ":" << A[a_idx] << std::endl;
	    ++a_idx;++b_idx;++offset_ba;
	  }
	}
	if (a_idx >= a_len && b_idx >= b_len) {
	  std::cout << "5:" << c_idx << ":" << C[c_idx] << std::endl;
	  seq.push_back(C[c_idx++]);
	} else if (b_idx >= b_len && c_idx >= c_len) {
	  std::cout << "6:" << a_idx << ":" << A[a_idx] << std::endl;
	  seq.push_back(A[a_idx++]);
	}
      }
      
      sequence mergedSeq(seq.begin(), seq.end());
      return mergedSeq;
    }

    bool isLcsElem () {
      
      
      
      return true;
    }
    
    bool isConflict_ () {
      return false;
      std::vector<int> ba_cidx  = diff_ba->getChangeIdx();
      std::vector<int>::iterator ba_cit = ba_cidx.begin();
      std::vector<int> bc_cidx  = diff_bc->getChangeIdx();
      std::vector<int>::iterator bc_cit = bc_cidx.begin();
      while (ba_cit != ba_cidx.end() && bc_cit != bc_cidx.end()) {
	if ((*ba_cit == SES_ADD    && *bc_cit == SES_ADD)    ||
	    (*ba_cit == SES_DELETE && *bc_cit == SES_COMMON)) {
	  return true;
	}
	++ba_cit;
	++bc_cit;
      }
      
      if (ba_cit != ba_cidx.end()) {
	int back = std::distance(ba_cit, ba_cidx.end());
	--bc_cit;
	while (back-- > 0) {
	  if ((*ba_cit == SES_ADD    && *bc_cit == SES_ADD)    ||
	      (*ba_cit == SES_DELETE && *bc_cit == SES_COMMON)) {
	    return true;
	  }
	  ++ba_cit;
	  --bc_cit;
	}
      }
      
      return false;
    }
  };
}

#endif // DTL_H
