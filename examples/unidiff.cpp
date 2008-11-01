
#include "../dtl.hpp"
#include "common.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SEPARATE_SIZE (3)
#define CONTEXT_SIZE (3)

void showStats (std::string fp1, std::string fp2) 
{
  const int MAX_LENGTH = 255;
  time_t rawtime[2];
  struct tm *timeinfo[2];
  char time_format[] = "%Y-%m-%d %H:%M:%S %z";
  struct stat st[2];

  if (stat(fp1.c_str(), &st[0]) == -1) {
    perror("argv1");
    exit(-1);
  }
  if (stat(fp2.c_str(), &st[1]) == -1) {
    perror("argv2");
    exit(-1);
  }

  char buf[2][MAX_LENGTH + 1];
  rawtime[0] = st[0].st_mtime;
  timeinfo[0] = localtime(&rawtime[0]);
  strftime(buf[0], MAX_LENGTH, time_format, timeinfo[0]);
  std::cout << "--- " << fp1 << '\t' << buf[0] << std::endl;
  rawtime[1] = st[1].st_mtime;
  timeinfo[1] = localtime(&rawtime[1]);
  strftime(buf[1], MAX_LENGTH, time_format, timeinfo[1]);
  std::cout << "+++ " << fp2 << '\t' << buf[1] << std::endl;
}

void unifiedDiff (std::string fp1, std::string fp2) 
{
  typedef std::string elem;
  std::ifstream Aifs(fp1.c_str());
  std::ifstream Bifs(fp2.c_str());
  elem buf;
  std::vector<elem> ALines, BLines;
  std::ostringstream ossLine, ossInfo;

  while(getline(Aifs, buf)){
    ALines.push_back(buf);
  }
  while(getline(Bifs, buf)){
    BLines.push_back(buf);
  }

  dtl::Diff<elem, std::vector<elem> > diff(ALines, BLines);
  diff.compose();

  // Short Edit Script
  typedef std::pair<elem, dtl::elemInfo> sesElem;
  dtl::Ses<elem> ses = diff.getSes();
  std::vector<sesElem> ses_v = ses.getSequence();
  std::vector<sesElem>::iterator it;
  it = ses_v.begin();

  std::vector<sesElem> common[2];
  std::vector<sesElem> change;
  int l_cnt = 1;
  int length = std::distance(ses_v.begin(), ses_v.end());
  int middle = 0;
  bool isMiddle, isAfter;
  isMiddle = isAfter = false;
  elem e;
  dtl::elemInfo einfo;
  int a, b, c, d;         // @@ -a,b +c,d @@
  a = b = c = d = 0;
  
  if (diff.getEditDistance() > 0) {
    showStats(fp1, fp2);             // show file info
  }

  for (it=ses_v.begin();it!=ses_v.end();++it, ++l_cnt) {
    e = it->first;
    einfo = it->second;
    switch (einfo.type) {
    case dtl::SES_ADD :
      middle = 0;
      change.push_back(*it);
      if (!isMiddle)       isMiddle = true;
      if (isMiddle)        ++d;
      if (l_cnt >= length) isAfter = true;
      break;
    case dtl::SES_DELETE :
      middle = 0;
      change.push_back(*it);
      if (!isMiddle)       isMiddle = true;
      if (isMiddle)        ++b;
      if (l_cnt >= length) isAfter = true;
      break;
    case dtl::SES_COMMON :
      ++b;++d;
      if (common[1].size() == 0 && change.size() == 0) {
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
	change.push_back(*it);
	if (middle >= SEPARATE_SIZE || l_cnt >= length) {
	  isAfter = true;
	}
      }
      break;
    default :
      // no through
      break;
    }
    // print unified format
    if (isAfter && change.size() > 0) {
      std::vector<sesElem>::iterator cit = it;
      int cnt = 0;
      for (int i=0;i<SEPARATE_SIZE;++i, ++cit) {
	if (cit->second.type == dtl::SES_COMMON) {
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
      if (diff.isReverse()) std::swap(a, c);
      std::cout << "@@ -" << a << "," << b << " +" << c << "," << d << " @@" << std::endl;
      std::vector<sesElem>::iterator vit;
      for (vit=common[0].begin();vit!=common[0].end();++vit) {
	std::cout << dtl::SES_MARK_COMMON << vit->first << std::endl;
      }
      for (vit=change.begin();vit!=change.end();++vit) {
	switch (vit->second.type) {
	case dtl::SES_ADD:
	  std::cout << dtl::SES_MARK_ADD    << vit->first << std::endl;
	  break;
	case dtl::SES_DELETE:
	  std::cout << dtl::SES_MARK_DELETE << vit->first << std::endl;
	  break;
	case dtl::SES_COMMON:
	  std::cout << dtl::SES_MARK_COMMON << vit->first << std::endl;
	  break;
	}
      }
      for (vit=common[1].begin();vit!=common[1].end();++vit) {
	std::cout << " " << vit->first << std::endl;
      }
      std::cout.flush();
      isMiddle = false;
      isAfter = false;
      common[0].clear();
      common[1].clear();
      change.clear();
      a = b = c = d = middle = 0;
    }
  }
}


int main(int argc, char *argv[])
{
  if (isFewArgs(argc)) {
    perror("few argument.");
    return(EXIT_FAILURE);
  }
  
  std::string s1(argv[1]);
  std::string s2(argv[2]);

  if (!fileExists(s1)) {
    perror("file A is not exist.");
    return(EXIT_FAILURE);
  }

  if (!fileExists(s2)) {
    perror("file B is not exist.");
    return(EXIT_FAILURE);
  }

  unifiedDiff(s1, s2);
  return 0;
}
