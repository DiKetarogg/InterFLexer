#ifndef DTG_UTILITIES_SORT
#define DTG_UTILITIES_SORT




namespace dtg {

template<class Type>
class MergeSort {
	public:
	MergeSort(Type* arr, size_t size, bool(*Compare)(const Type&, const Type&))
			:L(new Type[(size + 1)/ 2]),
			R(new Type[(size + 1)/ 2]),
			m_Array(arr),
			m_Compare(Compare){
		Sort(0, size - 1);
		clean();
	}

	private:

	void Sort(size_t l, size_t r) {
		if (l < r) {
			size_t m = l + (r-l)/2;
			Sort(l, m);
			Sort(m + 1, r);
			Merge(l, m, r);
		}
	}

	 void Merge(size_t l, size_t m, size_t r) {
		size_t n1 = m-l+1;
		size_t n2 = r-m;
		for(size_t i = 0;i < n1; ++i) {
			L[i] = m_Array[l+i]; 
		}
		for(size_t i = 0;i < n2; ++i) {
			R[i] = m_Array[m+1+i];
		}
		size_t i = 0, j = 0, k = l;
		while(i < n1 && j < n2) {
			if(m_Compare(L[i] , R[j])) {
				m_Array[k] = L[i];
				++k;
				++i;
			}
			else {
				m_Array[k] = R[j];
				++k;
				++j;
			}
			
		}
		while(i < n1) {
			m_Array[k] = L[i];
			++k;
			++i;
		}
		while(j < n2) {
			m_Array[k] = R[j];
			++k;
			++j;
		}
	}
	void clean() {
		delete[]L;
		delete[]R;
		L = 0;
		R = 0;
	}
	public:
	~MergeSort() {
		delete[]L;
		delete[]R;
	}
	
	private:
	Type* L;
	Type* R;
	Type*	m_Array;
	bool	(*m_Compare)(const Type&, const Type&);
};

}

#endif
