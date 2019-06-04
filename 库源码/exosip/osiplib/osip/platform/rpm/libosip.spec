Summary:	The GNU oSIP library
Summary(pl):	Biblioteka GNU oSIP
Name:		libosip2
Version:	4.1.0
Release:	1
License:	LGPL
Group:          Development/Libraries
Source0:	ftp://ftp.gnu.org/gnu/osip/%{name}-%{version}.tar.gz
URL:		http://www.gnu.org/software/osip/osip.html
BuildRequires:	autoconf
BuildRequires:	automake
BuildRequires:	libtool
BuildRoot:	/tmpdir/%{name}-%{version}-root-%(id -u -n)

%description
This is "the GNU oSIP library". It has been designed to provide
the Internet Community a simple way to support the Session Initiation
Protocol. SIP is described in the RFC3261 which is available at
http://www.ietf.org/rfc/rfc3261.txt.

%description -l pl
To jest biblioteka GNU oSIP. Zosta³a zaprojektowana, aby
dostarczyæ Spo³eczno¶ci Internetowej prost± obs³ugê protoko³u SIP.
Protokó³ SIP (Session Initiation Protocol) jest opisany w RFC3261.

%package devel
Summary:	The GNU oSIP library - development files
Summary(pl):	Pliki dla programistów u¿ywaj±cych GNU oSIP
Group:		Development/Libraries
Requires:	%{name} = %{version}

%description devel
Development files for the GNU oSIP library.

%description devel -l pl
Pliki dla programistów u¿ywaj±cych biblioteki GNU oSIP.

%package static
Summary:	The GNU oSIP library - static version
Summary(pl):	Statyczna biblioteka GNU oSIP
Group:		Development/Libraries
Requires:	%{name}-devel = %{version}

%description static
Static version of the GNU oSIP library.

%description static -l pl
Statyczna wersja biblioteki GNU oSIP.

%prep
%setup -q

%build
rm -f acinclude.m4 
%{__libtoolize}
%{__aclocal} -I scripts
%{__autoconf}
%{__automake}
%configure \
	--enable-pthread \
	--%{?debug:en}%{!?debug:dis}able-debug

%{__make}

%install
rm -rf $RPM_BUILD_ROOT

%{__make} install \
	DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%post   -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_libdir}/lib*.so.*.*

%files devel
%defattr(644,root,root,755)
%doc AUTHORS BUGS ChangeLog NEWS README TODO
%attr(755,root,root) %{_libdir}/lib*.la
%attr(755,root,root) %{_libdir}/lib*.so
%{_includedir}/*

%files static
%defattr(644,root,root,755)
%{_libdir}/lib*.a

%define date	%(echo `LC_ALL="C" date +"%a %b %d %Y"`)
