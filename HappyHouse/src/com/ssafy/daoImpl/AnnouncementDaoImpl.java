package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.AnnouncementDao;
import com.ssafy.dto.AnnouncementDto;
import com.ssafy.util.DBUtil;

public class AnnouncementDaoImpl implements AnnouncementDao {

	private static AnnouncementDao annDao = new AnnouncementDaoImpl();

	private DBUtil dbUtil;

	private AnnouncementDaoImpl() {
		dbUtil = DBUtil.getInstance();
	}

	public static AnnouncementDao getAnnDao() {
		return annDao;
	}

	@Override
	public void writeAnnouncement(AnnouncementDto annDto) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "insert into announcement (title, content, writer, date) \n" + "values (?, ?, ?, now())";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, annDto.getTitle());
			pstmt.setString(2, annDto.getContent());

			pstmt.executeUpdate();
		} finally {
			dbUtil.close(pstmt, conn);
		}

	}

	@Override
	public List<AnnouncementDto> listAnnouncement(String word) throws Exception {
		List<AnnouncementDto> list = new ArrayList<AnnouncementDto>();
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;

		try {
			conn = dbUtil.getConnection();
			/*
			 * String sql = "select a.no, a.title, a.content, a.writer, a.date \n" +
			 * "from announcement a, user u \n" + "where a.writer = u.username ";
			 */
			String sql = "select * from announcement \n";
			if (!word.isEmpty()) {
				/*
				 * if (key.equals("writer")) sql += "where a.writer = ? \n"; else if
				 * (key.equals("title")) sql += "and a.title like ? \n";
				 */
				sql += "where title like ? or content like ? \n";
			}
			sql += "order by no desc";
			pstmt = conn.prepareStatement(sql);
			if (!word.isEmpty()) {
				/*
				 * if (key.equals("writer")) pstmt.setString(1, word); else if
				 * (key.equals("title"))
				 */
				pstmt.setString(1, "%" + word + "%");
				pstmt.setString(2, "%" + word + "%");
			}
			rs = pstmt.executeQuery();
			while (rs.next()) {
				AnnouncementDto annDto = new AnnouncementDto();
				annDto.setNo(rs.getInt("no"));
				annDto.setTitle(rs.getString("title"));
				annDto.setContent(rs.getString("content"));
				annDto.setWriter(rs.getString("writer"));
				annDto.setDate(rs.getString("date"));

				list.add(annDto);
			}
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return list;
	}

	@Override
	public AnnouncementDto getAnnouncement(int annNo) throws Exception {
		AnnouncementDto annDto = null;
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "select title, content, writer, date \n" + "from announcement \n" + "where no = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setInt(1, annNo);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				annDto = new AnnouncementDto();
				annDto.setNo(annNo);
				annDto.setTitle(rs.getString("title"));
				annDto.setContent(rs.getString("content"));
				annDto.setWriter(rs.getString("writer"));
				annDto.setDate(rs.getString("date"));
			}
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}

		return annDto;
	}

	@Override
	public void updateAnnouncement(AnnouncementDto annDto) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "update announcement \n" + "set title = ?, content = ?, date = now() \n" + "where no = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, annDto.getTitle());
			pstmt.setString(2, annDto.getContent());
			pstmt.setInt(3, annDto.getNo());
			pstmt.executeUpdate();
		} finally {
			dbUtil.close(pstmt, conn);
		}

	}

	@Override
	public void deleteAnnouncement(int annNo) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "delete from announcement \n" + "where no = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setInt(1, annNo);
			pstmt.executeUpdate();
		} finally {
			dbUtil.close(pstmt, conn);
		}

	}

}
