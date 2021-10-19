package com.ssafy.controller;

import java.io.IOException;
import java.util.List;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;

import com.ssafy.dto.AnnouncementDto;
import com.ssafy.dto.UserDto;

import service.AnnouncementService;
import service.AnnouncementServiceImpl;

@WebServlet("/announcement")
public class AnnouncementController extends HttpServlet {
	private static final long serialVersionUID = 1L;

	private AnnouncementService annService;

	public void init() {
		annService = AnnouncementServiceImpl.getAnnService();
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
		if ("list".equals(act)) {
			path = listAnnouncement(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		} else if ("mvmodify".equals(act)) {
			path = getAnnouncement(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		} else if ("modify".equals(act)) {
			path = modifyAnnouncement(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		} else if ("delete".equals(act)) {
			path = deleteAnnouncement(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		} else if ("mvwrite".equals(act)) {
			path = root + "/jsp/notice/write.jsp";
			response.sendRedirect(path);
		} else if ("write".equals(act)) {
			path = writeAnnouncement(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		} else if ("detail".equals(act)) {
			path = getAnnouncement(request, response);
			RequestDispatcher dispatcher = request.getRequestDispatcher(path);
			dispatcher.forward(request, response);
		}

		/*
		 * else if ("register".equals(act)) { path = registerArticle(request, response);
		 * RequestDispatcher dispatcher = request.getRequestDispatcher(path);
		 * dispatcher.forward(request, response); } else if ("delete".equals(act)) {
		 * path = deleteArticle(request, response); RequestDispatcher dispatcher =
		 * request.getRequestDispatcher(path); dispatcher.forward(request, response); }
		 * else if ("mvmodify".equals(act)) { path = getArticle(request, response);
		 * RequestDispatcher dispatcher = request.getRequestDispatcher(path);
		 * dispatcher.forward(request, response); } else if ("modify".equals(act)) {
		 * path = modifyArticle(request, response); RequestDispatcher dispatcher =
		 * request.getRequestDispatcher(path); dispatcher.forward(request, response); }
		 * else if ("".equals(act)) {
		 * 
		 * }
		 */
	}

	private String writeAnnouncement(HttpServletRequest request, HttpServletResponse response) {
		HttpSession session = request.getSession();
		UserDto userDto = (UserDto) session.getAttribute("userinfo");
		if (userDto != null && userDto.getUsername().equals("admin")) {
			AnnouncementDto annDto = new AnnouncementDto();
			annDto.setTitle(request.getParameter("title"));
			annDto.setContent(request.getParameter("content"));
			annDto.setWriter(request.getParameter("writer"));

			try {
				annService.writeAnnouncement(annDto);
				return "/announcement?act=list";
			} catch (Exception e) {
				e.printStackTrace();
				request.setAttribute("msg", "글작성 중 문제 발생!!!");
				return "/error/error.jsp";
			}

		} else
			return "/user?act=login";
	}

	private String deleteAnnouncement(HttpServletRequest request, HttpServletResponse response) {
		int annNo = Integer.parseInt(request.getParameter("no"));
		HttpSession session = request.getSession();
		UserDto userDto = (UserDto) session.getAttribute("userinfo");
		if (userDto != null && userDto.getUsername().equals("admin")) {
			try {
				annService.deleteAnnouncement(annNo);
				request.setAttribute("msg", "공지 삭제 성공!!!!");
				return "/announcement?act=list";
			} catch (Exception e) {
				e.printStackTrace();
				request.setAttribute("msg", "글삭제 중 문제 발생!!!");
				return "/error/error.jsp";
			}
		} else
			return "/user?act=login";
	}

	private String modifyAnnouncement(HttpServletRequest request, HttpServletResponse response) {
		HttpSession session = request.getSession();
		UserDto userDto = (UserDto) session.getAttribute("userinfo");
		if (userDto != null && userDto.getUsername().equals("admin")) {
			try {
				AnnouncementDto annDto = new AnnouncementDto();
				annDto.setTitle(request.getParameter("title"));
				annDto.setContent(request.getParameter("content"));
				annDto.setNo(Integer.parseInt(request.getParameter("no")));
				annService.updateAnnouncement(annDto);
				request.setAttribute("msg", "공지 수정 성공!!!!");
				return "/announcement?act=list";
			} catch (Exception e) {
				e.printStackTrace();
				request.setAttribute("msg", "글수정 중 문제 발생!!!");
				return "/error/error.jsp";
			}
		} else
			return "/user?act=login";
	}

	private String getAnnouncement(HttpServletRequest request, HttpServletResponse response) {
		String act = request.getParameter("act");
		int annNo = Integer.parseInt(request.getParameter("no"));
		HttpSession session = request.getSession();
		UserDto userDto = (UserDto) session.getAttribute("userinfo");
		if ("mvmodify".equals(act)) {
			if (userDto != null && userDto.getUsername().equals("admin")) {
				try {
					AnnouncementDto annDto = annService.getAnnouncement(annNo);
					request.setAttribute("ann", annDto);
					return "jsp/notice/modify.jsp";
				} catch (Exception e) {
					e.printStackTrace();
					request.setAttribute("msg", "공지 얻기 중 문제 발생!!!");
					return "/error/error.jsp";
				}
			} else
				return "/jsp/login.jsp";
		} else if ("detail".equals(act)) {
			try {
				AnnouncementDto annDto = annService.getAnnouncement(annNo);
				request.setAttribute("ann", annDto);
				return "jsp/notice/detail.jsp";
			} catch (Exception e) {
				e.printStackTrace();
				request.setAttribute("msg", "공지 얻기 중 문제 발생!!!");
				return "/error/error.jsp";
			}

		} else
			return "/index.jsp";
	}

	private String listAnnouncement(HttpServletRequest request, HttpServletResponse response) {
		String word = request.getParameter("word"); // 검색어

		try {
			HttpSession session = request.getSession();
			UserDto userDto = (UserDto) session.getAttribute("userinfo");
			if (userDto != null) {

				List<AnnouncementDto> list = annService.listAnnouncement(word);

				request.setAttribute("anns", list);

				return "/jsp/announcements.jsp";
			} else
				return "/jsp/login.jsp";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute("msg", "공지사항 목록 출력 중 문제 발생!!!");
			return "/error/error.jsp";
		}
	}
}
