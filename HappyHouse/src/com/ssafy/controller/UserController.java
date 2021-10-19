package com.ssafy.controller;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;

import com.ssafy.dto.UserDto;

import service.UserService;
import service.UserServiceImpl;

@WebServlet("/user")
public class UserController extends HttpServlet {
	private static final long serialVersionUID = 1L;

	private UserService userService;

	public void init() {
		userService = UserServiceImpl.getUserService();
	}

	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		process(request, response);
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		request.setCharacterEncoding("utf-8");
		process(request, response);
	}

	private void process(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		String root = request.getContextPath();

		String act = request.getParameter("act");
		String path = "/index.jsp";
		if ("mvregister".equals(act)) {
			path = "/user/join.jsp";
			response.sendRedirect(root + path);
		} else if ("edit-userinfo".equals(act)) {
			path = editUserInfo(request, response);
			response.sendRedirect(root + path);
		} else if ("idcheck".equals(act)) {
			path = idCheck(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		} else if ("register-user".equals(act)) {
			path = registerUser(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		} else if ("login".equals(act)) {
			path = loginUser(request, response);
			if ("/index.jsp".equals(path)) {
				response.sendRedirect(root + path);
			} else {
				RequestDispatcher dispatcher = request.getRequestDispatcher(path);
				dispatcher.forward(request, response);
			}
		} else if ("logout".equals(act)) {
			path = logOut(request, response);
			System.out.println("logout");
			response.sendRedirect(root + path);
		} else if ("".equals(act)) {

		} else if ("".equals(act)) {

		} else if ("".equals(act)) {

		}
	}

	private String editUserInfo(HttpServletRequest request, HttpServletResponse response) {
		UserDto userDto = new UserDto();
		userDto.setUsername(request.getParameter("username"));
		userDto.setName(request.getParameter("name"));
		userDto.setPassword(request.getParameter("password"));
		userDto.setEmail(request.getParameter("email"));
		userDto.setPhone(request.getParameter("phone"));
		userDto.setAddress(request.getParameter("address"));
		userDto.setAddressDetail(request.getParameter("address-detail"));
		try {
			userService.updateUser(userDto);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("msg", "회원 정보 수정 중 문제 발생!!!");
			return "/jsp/mypage.jsp";
		}
		return "/index.jsp";
	}

	private String logOut(HttpServletRequest request, HttpServletResponse response) {
		HttpSession session = request.getSession();
		session.invalidate();
		return "/index.jsp";
	}

	private String loginUser(HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("username");
		String pass = request.getParameter("password");
		UserDto userDto = null;
		try {
			userDto = userService.login(id, pass);

			if (userDto != null) {
//				session 설정
				HttpSession session = request.getSession();
				session.setAttribute("userinfo", userDto);

//				cookie 설정
				String idsave = request.getParameter("idsave");
				if ("saveok".equals(idsave)) {// 아이디 저장을 체크 했다면.
					Cookie cookie = new Cookie("username", id);
					cookie.setPath(request.getContextPath());
					cookie.setMaxAge(60 * 60 * 24 * 365 * 40);// 40년간 저장.
					response.addCookie(cookie);
				} else {// 아이디 저장을 해제 했다면.
					Cookie cookies[] = request.getCookies();
					if (cookies != null) {
						for (Cookie cookie : cookies) {
							if ("username".equals(cookie.getName())) {
								cookie.setMaxAge(0);
								response.addCookie(cookie);
								break;
							}
						}
					}
				}
				request.removeAttribute("act");
				return "/index.jsp";
			} else {
				request.setAttribute("msg", "아이디 또는 비밀번호 확인 후 다시 로그인하세요!");
				return "/jsp/login.jsp";
			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("msg", "로그인 중 문제 발생!!!");
			return "/jsp/login.jsp";
		}
	}

	private String idCheck(HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("ckid");
		int cnt = 1;
		try {
			cnt = userService.usernameCheck(id);
			request.setAttribute("idcnt", cnt);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return "/jsp/username_check.jsp";
	}

	private String registerUser(HttpServletRequest request, HttpServletResponse response) {
		UserDto userDto = new UserDto();
		userDto.setUsername(request.getParameter("username"));
		userDto.setName(request.getParameter("name"));
		userDto.setPassword(request.getParameter("password"));
		userDto.setEmail(request.getParameter("email"));
		userDto.setPhone(request.getParameter("phone"));
		userDto.setAddress(request.getParameter("address"));
		userDto.setAddressDetail(request.getParameter("address-detail"));

		try {
			userService.register(userDto);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("msg", "회원 가입 중 문제 발생!!!");
			return "/jsp/register.jsp";
		}
		return "/jsp/login.jsp";
	}

}
