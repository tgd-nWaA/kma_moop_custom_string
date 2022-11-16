
#define NDEBUG

#include <string>

class bad_index : std::exception {};
class bad_pointer : std::exception {};

template <typename CharT, typename Traits = std::char_traits<CharT>>
class m_basic_string {
public:

	//template <typename CharT, typename Traits = std::char_traits<CharT>>
	class string_rep {

		friend class m_basic_string;

	public:
		string_rep(void);
		string_rep(const CharT c);
		string_rep(const CharT* s);
		/*string_rep(std::string&);*/

		~string_rep();

		//pseudo-copying constructor
		/*string_rep* get_copy();*/
		string_rep* get_copy();
		void assign(size_t len, const CharT* s);

		string_rep(const string_rep&) = delete;
		string_rep& operator=(const string_rep&) = delete;

		inline size_t use_count() const
		{return use_count_;};

		inline size_t len() const
		{return len_;};

		inline CharT* str() const
		{
			return str_;
		};

	private:

		size_t len_;
		CharT* str_;
		size_t use_count_;
		/*bool shareable_;*/
	};


	//template <typename CharT, typename Traits = std::char_traits<CharT>>
	class proxy
	{
	public:

		proxy(m_basic_string*, size_t pos);

		inline operator CharT() const;

		inline ~proxy() {};

		proxy operator= (const CharT& value);

	private:
		m_basic_string* b_str_;
		size_t          pos_;
	};

	//converting
	m_basic_string(void);
	m_basic_string(const CharT c);
	m_basic_string(const CharT* s);
	m_basic_string(std::basic_string<CharT>& s);

	//copy
	m_basic_string(const m_basic_string& c);
	m_basic_string& operator=(m_basic_string& s)&;

	//move
	m_basic_string(m_basic_string&& s) noexcept;
	m_basic_string& operator=(m_basic_string&& s) noexcept;

	~m_basic_string();

	m_basic_string& operator+=(const m_basic_string& s)&;

	//todo correct?
	proxy operator[](size_t i);

	CharT operator[](size_t i) const;

	m_basic_string tolower() const;
	m_basic_string touppper() const;


	m_basic_string<CharT> operator+(const m_basic_string<CharT>& r) const
	{
		return l += r;
	};

	bool operator==(const m_basic_string<CharT>& l, const m_basic_string<CharT>& r)
	{
		return (Traits::compare(l.c_str(), r.c_str(), l.len()) == 0);
	};

	bool operator!=(const m_basic_string<CharT>& l, const m_basic_string<CharT>& r)
	{
		return !(l == (r));
	};

	bool operator<(const m_basic_string<CharT>& l, const m_basic_string<CharT>& r)
	{
		return (Traits::compare(l.c_str(), r.c_str(), l.len()) < 0);
	};

	bool operator>(const m_basic_string<CharT>& l, const m_basic_string<CharT>& r)
	{
		return !(l < r);
	};

	bool operator<=(const m_basic_string<CharT>& l, const m_basic_string<CharT>& r)
	{
		return l < r || l == r;
	};

	bool operator>=(const m_basic_string<CharT>& l, const m_basic_string<CharT>& r)
	{
		return !(l < r) || l == r;
	};


	inline size_t len() const
	{
		return rep_->len();
	};

	inline const CharT* c_str() const
	{
		return rep_->str();
	};

	inline bool empty() const
	{
		return rep_->len() == 0;
	};

	explicit inline operator const char* () const
	{
		return c_str();
	};

	explicit inline operator
		std::basic_string<CharT>() const
	{
		return std::basic_string<CharT>(c_str());
	};

	/*inline const char* begin() const
	{
		return &this->c_str()[0];
	};

	inline const char* end() const
	{
		return &this->c_str()[length + 1];
	};*/

private:
	
	string_rep* rep_;
};

template <typename CharT, typename Traits = std::char_traits<CharT>>
std::basic_ostream<CharT>&
operator<<(std::basic_ostream<CharT>& os, const m_basic_string<CharT>& str)
{
	return os << str.c_str();
};

//template <typename T, typename S>
//void swap_with(m_basic_string<T, S>& l, m_basic_string<T, S>& r)
//{
//	l.swap_with(r);
//}

using m_string = m_basic_string<char>;
using wm_string = m_basic_string<wchar_t>;
using u16m_string = m_basic_string<char16_t>;
using u32m_string = m_basic_string<char32_t>;

/*
*	m_basic_string
*/

//default constructor
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::m_basic_string(void)
	: rep_(new string_rep())
{
	#ifndef NDEBUG
	std::cout << "default constructor" << "\n";
	#endif
};

//converting constructor for CharT
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::m_basic_string(const CharT c)
	: rep_(new string_rep(c))
{
	#ifndef NDEBUG
	std::cout << "converting constructor for CharT" << "\n";
	#endif
};

//converting constructor for CharT
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::~m_basic_string()
{
	if (--rep_->use_count_ == 0)
		delete rep_;

	#ifndef NDEBUG
	std::cout << "destructor" << "\n";
	#endif  
};

//converting constructor for c-style strings 
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::m_basic_string(const CharT* s)
	: rep_(new string_rep(s))
{
	#ifndef NDEBUG
	std::cout << "converting constructor for c-style strings " << "\n";
	#endif  
};

//converting constructor for std::basic_string<CharT>
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::
m_basic_string(std::basic_string<CharT>& s)
	: m_basic_string(s.c_str())
{
	#ifndef NDEBUG
	std::cout << "converting constructor for std::basic_string<CharT>" << "\n";
	#endif  
};

//copy constructor
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::
m_basic_string(const m_basic_string& c)
	: m_basic_string()
{
	rep_ = c.rep_->get_copy();
	#ifndef NDEBUG
	std::cout << "copy constructor" << "\n";
	#endif    
};

//copy assignment
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>&
m_basic_string<CharT, Traits>::
operator=(m_basic_string& s)&
{
	++s.rep_->use_count_;

	if (--rep_->use_count_ == 0)
		delete rep_;

	rep_ = s.rep_;

	return *this;

	#ifndef NDEBUG
	std::cout << "copy assignment" << "\n";
	#endif    
};

//move constructor
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::
m_basic_string(m_basic_string&& s) noexcept
	: m_basic_string()
{
	std::swap(rep_, s.rep_);
#ifndef NDEBUG
	std::cout << "move constructor" << "\n";
#endif    
};

//move assignment
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>&
m_basic_string<CharT, Traits>::
operator=(m_basic_string&& s) noexcept
{
	++s.rep_->use_count_;

	if (--rep_->use_count_ == 0)
		delete rep_;

	rep_ = s.rep_;

	return *this;

	#ifndef NDEBUG
	std::cout << "move assignment" << "\n";
	#endif    
};

//operator +=
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>&
m_basic_string<CharT, Traits>::
operator+=(const m_basic_string& s)&
{
	size_t curr_len = len();
	size_t add_len = s.len();
	size_t total_len = curr_len + add_len;

	CharT* s_res = new CharT[total_len + 1];
	Traits::copy(s_res, rep_->str_, curr_len);
	Traits::copy(s_res + curr_len, s.rep_->str_, add_len + 1);

	string_rep* res(new string_rep(s_res));

	if (--rep_->use_count_ == 0)
		delete rep_;

	rep_ = res;

	return *this;

#ifndef NDEBUG
	std::cout << "move assignment" << "\n";
#endif    
};

//modifier[]
template <typename CharT, typename Traits>
typename m_basic_string<CharT, Traits>::proxy
m_basic_string<CharT, Traits>::
operator[](size_t i)
{
	return proxy(this, i);

#ifndef NDEBUG
	std::cout << "move assignment" << "\n";
#endif    
};

//selector[]
template <typename CharT, typename Traits>
CharT m_basic_string<CharT, Traits>::
operator[](size_t i) const 
{
	if (i > len())
		throw bad_index();

	return *(rep_->str_ + i);

	#ifndef NDEBUG
	std::cout << "move assignment" << "\n";
	#endif    
};

template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>
m_basic_string<CharT, Traits>::
tolower() const
{
	size_t len = rep_->len();
	CharT* buffer = new CharT[len + 1];

	for (int i = 0; i < len; i++)
		buffer[i] = std::tolower(rep_->str_[i]);

	return m_basic_string(buffer);
}

template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>
m_basic_string<CharT, Traits>::
touppper() const
{
	size_t len = rep_->len();
	CharT* buffer = new CharT[len + 1];

	for (int i = 0; i < len; i++)
		buffer[i] = std::toupper(rep_->str_[i]);

	return m_basic_string(buffer);
}



/*
*	string_rep
*/

//default constructor
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::
string_rep::string_rep(void)
	: len_(0)
	, str_(new CharT[1])
	, use_count_(1)
{
	*str_ = '\0';
};

//converting constructor for CharT
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::
string_rep::string_rep(const CharT c)
	: len_(1)
	, str_(new CharT[2])
	, use_count_(1)
{
	*str_ = c;
	*(str_ + 1) = '\0';
};

//converting constructor for c-style strings
template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::
string_rep::string_rep(const CharT* s)
	: len_(Traits::length(s))
	, str_(new CharT[len_ + 1])
	, use_count_(1)
{
	Traits::copy(str_, s, len_ + 1);
};

template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::
string_rep::~string_rep()
{
	delete str_;
};

template <typename CharT, typename Traits>
typename m_basic_string<CharT, Traits>::string_rep*
m_basic_string<CharT, Traits>::
string_rep::get_copy()
{
	++use_count_;
	return this;
};

//string_rep(const string_rep* rep)
//	: len_(rep->len())
//	, str_(str_->c_str())
//	, use_count_(++rep->use_count())
//{};

//pseudo-ajssignment
template <typename CharT, typename Traits>
void m_basic_string<CharT, Traits>::
string_rep::assign(size_t len, const CharT* s)
{
	if (s == nullptr)
		throw bad_pointer();

	CharT* res = new CharT[len + 1];
	Traits::copy(res, s, len + 1);

	len_ = len;
	use_count_ = 1;
	str_ = res;

	/*if (this->use_count_ == 1)
		delete this;
	else
		--this->use_count_;*/
};

/*
*	proxy
*/

template <typename CharT, typename Traits>
m_basic_string<CharT, Traits>::proxy::
proxy(m_basic_string* s, size_t pos)
	: b_str_(s)
	, pos_(pos)
{};

template <typename CharT, typename Traits>
inline m_basic_string<CharT, Traits>::proxy::
operator CharT() const
{
	if (pos_ > b_str_->rep_->len())
		throw bad_index();

	return *(b_str_->rep_->str_ + pos_);
};

template <typename CharT, typename Traits>
typename m_basic_string<CharT, Traits>::proxy
m_basic_string<CharT, Traits>::
proxy::operator=(const CharT& value)
{
if (pos_ > b_str_->rep_->len())
		throw bad_index();

	if (b_str_->rep_->use_count() == 1)
		*(b_str_->rep_->str() + pos_) = value;

	if (--b_str_->rep_->use_count_ >= 1)
	{
		size_t len = b_str_->rep_->len();
		CharT* res = new CharT[len + 1];
		Traits::copy(res, b_str_->rep_->str_, len + 1);
		*(res + pos_) = value;

		b_str_->rep_ = new string_rep(res);

	}

	return *this;
};
