-- phpMyAdmin SQL Dump
-- version 2.11.7
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jun 11, 2009 at 08:53 PM
-- Server version: 5.0.51
-- PHP Version: 5.2.4-2ubuntu5.6

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `rautemusik`
--

-- --------------------------------------------------------

--
-- Table structure for table `gb`
--

CREATE TABLE IF NOT EXISTS `gb` (
  `id` int(11) NOT NULL auto_increment,
  `username` varchar(20) NOT NULL,
  `mail` varchar(255) NOT NULL,
  `subject` varchar(255) NOT NULL,
  `message` text NOT NULL,
  `ts` timestamp NOT NULL default CURRENT_TIMESTAMP,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Dumping data for table `gb`
--

